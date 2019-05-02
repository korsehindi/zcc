#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void **data;
  int len;
  int cap;
} Vec;

enum {
  TK_NUM = 256,
  TK_EQ, // ==
  TK_NE, // !=
  TK_LE, // <=
  TK_GE, // >=
  TK_L,  // <
  TK_G,  // >
  TK_EOF,
};

typedef struct {
  int ty;
  int val;
  char *input;
} Token;

enum {
  ND_NUM = 256,
  ND_EQ, // ==
  ND_NE, // !=
  ND_L,  // <
  ND_LE, // <=
};

typedef struct Node {
  int ty;
  int val;
  struct Node *lhs;
  struct Node *rhs;
} Node;

// tokenizer.c
Vec *tokenize(char *s);

// parser.c
Vec *parse(Vec *tokens);

// gen_x86.c
void gen_x86(Vec *prog);

// util.c
void err(char *fmt, ...);
Vec *new_vec();
void vec_push(Vec *v, void *x);

// util_test.c
void util_test();
