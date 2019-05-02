#include "zcc.h"

static Vec *tokens;
static int pos = 0;

static Node *equality();

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

static int is_eof() {
  Token *t = tokens->data[pos];
  return t->ty == TK_EOF;
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

Vec *parse(Vec *tokens_) {
  tokens = tokens_;
  pos = 0;
  Vec *v = new_vec();

  while (!is_eof())
    vec_push(v, equality());

  vec_push(v, NULL);
  return v;
}
