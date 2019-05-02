#include "zcc.h"

Token tokens[100];

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

void tokenize(char *s) {
  int i = 0;

  while (*s) {
    if (isspace(*s)) {
      s++;
      continue;
    }
    if (*s == '+' || *s == '-') {
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

int main(int argc, char **argv) {
  if (argc != 2)
    err("Usage: zcc <code>");

  tokenize(argv[1]);

  p(".intel_syntax noprefix");

#ifdef __APPLE__
  p(".global _main");
  p("_main:");
#else
  p(".global main");
  p("main:");
#endif

  int i = 1;

  if (tokens[0].ty != TK_NUM)
    err("invalid token: %s", tokens[0].input);
  tp("mov rax, %d", tokens[0].val);

  while (tokens[i].ty != TK_EOF) {
    if (tokens[i].ty == '+') {
      i++;
      if (tokens[i].ty != TK_NUM)
        err("invalid token: %s", tokens[i].input);
      tp("add rax, %d", tokens[i].val);
      i++;
      continue;
    }
    if (tokens[i].ty == '-') {
      i++;
      if (tokens[i].ty != TK_NUM)
        err("invalid token: %s", tokens[i].input);
      tp("sub rax, %d", tokens[i].val);
      i++;
      continue;
    }
    err("invalid token: %s", tokens[i].input);
  }

  tp("ret");
  return 0;
}
