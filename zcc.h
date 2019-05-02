#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
