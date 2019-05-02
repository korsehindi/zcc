#include "zcc.h"

Vec *tokens;
int pos = 0;

static Node *equality();

static void p(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  printf("\n");
}

static void tp(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  printf("\t");
  vprintf(fmt, ap);
  printf("\n");
}

static Node *new_node(int op, Node *lhs, Node *rhs) {
  Node *n = malloc(sizeof(Node));
  n->ty = op;
  n->lhs = lhs;
  n->rhs = rhs;
  return n;
}

static void expect(int ty) {
  Token *t = tokens->data[pos];
  if (t->ty != ty)
    err("%c expected, but got %c", ty, t->ty);
  pos++;
}

static int consume(int ty) {
  Token *t = tokens->data[pos];
  if (t->ty != ty)
    return 0;
  pos++;
  return 1;
}

static struct {
  char *name;
  int ty;
} symbols[] = {
    {"==", TK_EQ}, {"!=", TK_NE}, {"<=", TK_LE}, {">=", TK_GE},
    {"<", TK_L},   {">", TK_G},   {NULL, 0},
};

static Token *add_token(Vec *v, int ty, char *input) {
  Token *t = malloc(sizeof(Token));
  t->ty = ty;
  t->input = input;
  vec_push(v, t);
  return t;
}

Vec *tokenize(char *s) {
  Vec *v = new_vec();

loop:
  while (*s) {
    if (isspace(*s)) {
      s++;
      continue;
    }
    for (int i = 0; symbols[i].name; i++) {
      char *name = symbols[i].name;
      int len = strlen(name);
      if (strncmp(s, name, len))
        continue;
      add_token(v, symbols[i].ty, s);
      s += len;
      goto loop;
    }
    if (strchr("+-*/()", *s)) {
      add_token(v, *s, s);
      s++;
      continue;
    }
    if (isdigit(*s)) {
      Token *t = add_token(v, TK_NUM, s);
      t->val = strtol(s, &s, 10);
      continue;
    }
    err("can not tokenize: '%s'", s);
  }

  add_token(v, TK_EOF, s);
  return v;
}

static Node *term() {
  if (consume('(')) {
    Node *n = equality();
    expect(')');
    return n;
  }
  Token *t;
  t = tokens->data[pos];
  if (t->ty == TK_NUM) {
    t = tokens->data[pos++];
    Node *n = malloc(sizeof(Node));
    n->ty = ND_NUM;
    n->val = t->val;
    return n;
  }
  err("invalid token %s", t->input);
  return NULL;
}

Node *unary() {
  if (consume('+'))
    return term();
  if (consume('-')) {
    Node *n = malloc(sizeof(Node));
    n->ty = ND_NUM;
    n->val = 0;
    return new_node('-', n, term());
  }
  return term();
}

static Node *mul() {
  Node *n = unary();
  for (;;) {
    if (consume('*'))
      n = new_node('*', n, unary());
    else if (consume('/'))
      n = new_node('/', n, unary());
    else
      return n;
  }
}

static Node *add() {
  Node *n = mul();
  for (;;) {
    if (consume('+'))
      n = new_node('+', n, mul());
    else if (consume('-'))
      n = new_node('-', n, mul());
    else
      return n;
  }
}

static Node *relational() {
  Node *n = add();
  for (;;) {
    if (consume(TK_L))
      n = new_node(ND_L, add(), n);
    else if (consume(TK_LE))
      n = new_node(ND_LE, add(), n);
    else if (consume(TK_G))
      n = new_node(ND_L, n, add());
    else if (consume(TK_GE))
      n = new_node(ND_LE, n, add());
    return n;
  }
}

static Node *equality() {
  Node *n = relational();
  for (;;) {
    if (consume(TK_EQ))
      n = new_node(ND_EQ, n, relational());
    else if (consume(TK_NE))
      n = new_node(ND_NE, n, relational());
    return n;
  }
}

void gen(Node *n) {
  if (n->ty == ND_NUM) {
    tp("push %d", n->val);
    return;
  }

  gen(n->lhs);
  gen(n->rhs);
  tp("pop rdi");
  tp("pop rax");

  switch (n->ty) {
  case '+':
    tp("add rax, rdi");
    break;
  case '-':
    tp("sub rax, rdi");
    break;
  case '*':
    tp("mul rdi");
    break;
  case '/':
    tp("mov rdx, 0");
    tp("div rdi");
    break;
  case ND_EQ:
    tp("cmp rdi, rax");
    tp("sete al");
    tp("movzx rax, al");
    break;
  case ND_NE:
    tp("cmp rdi, rax");
    tp("setne al");
    tp("movzx rax, al");
    break;
  case ND_L:
    tp("cmp rdi, rax");
    tp("setl al");
    tp("movzx rax, al");
    break;
  case ND_LE:
    tp("cmp rdi, rax");
    tp("setle al");
    tp("movzx rax, al");
    break;
  }

  tp("push rax");
}

int main(int argc, char **argv) {
  if (argc != 2)
    err("Usage: zcc <code>");

  if (argc == 2 && !strcmp(argv[1], "-test")) {
    util_test();
    return 0;
  }

  tokens = tokenize(argv[1]);
  Node *node = equality();

  p(".intel_syntax noprefix");

#ifdef __APPLE__
  p(".global _main");
  p("_main:");
#else
  p(".global main");
  p("main:");
#endif

  gen(node);
  tp("pop rax");
  tp("ret");
  return 0;
}
