#include "zcc.h"

Token tokens[100];
int pos = 0;

static Node *add();

void err(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

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

static int consume(int ty) {
  if (tokens[pos].ty != ty)
    return 0;
  pos++;
  return 1;
}

void tokenize(char *s) {
  int i = 0;

  while (*s) {
    if (isspace(*s)) {
      s++;
      continue;
    }
    if (strchr("+-*/()", *s)) {
      tokens[i].ty = *s;
      tokens[i].input = s;
      i++;
      s++;
      continue;
    }
    if (isdigit(*s)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = s;
      tokens[i].val = strtol(s, &s, 10);
      i++;
      continue;
    }
    err("can not tokenize: '%s'", s);
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = s;
}

static Node *term() {
  if (consume('(')) {
    Node *n = add();
    if (!consume(')'))
      err(") expected, but got: %s", tokens[pos].input);
    return n;
  }
  if (tokens[pos].ty == TK_NUM) {
    Node *n = malloc(sizeof(Node));
    n->ty = ND_NUM;
    n->val = tokens[pos++].val;
    return n;
  }
  err("invalid token %s", tokens[pos].input);
  return NULL;
}

static Node *mul() {
  Node *n = term();
  for (;;) {
    if (consume('*'))
      n = new_node('*', n, term());
    else if (consume('/'))
      n = new_node('/', n, term());
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
  }

  tp("push rax");
}

int main(int argc, char **argv) {
  if (argc != 2)
    err("Usage: zcc <code>");

  tokenize(argv[1]);
  Node *node = add();

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
