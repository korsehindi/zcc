#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum {
  TK_NUM = 256,
  TK_EOF,
};

typedef struct {
  int ty;
  int val;
  char *input;
} Token;
