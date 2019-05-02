#include "zcc.h"

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

int main(int argc, char **argv) {
  if (argc != 2)
    err("Usage: zcc <code>");

  char *s = argv[1];

  p(".intel_syntax noprefix");

#ifdef __APPLE__
  p(".global _main");
  p("_main:");
#else
  p(".global main");
  p("main:");
#endif

  tp("mov rax, %ld", strtol(s, &s, 10));
  while (*s) {
    if (*s == '+') {
      s++;
      tp("add rax, %ld", strtol(s, &s, 10));
      continue;
    }
    if (*s == '-') {
      s++;
      tp("sub rax, %ld", strtol(s, &s, 10));
      continue;
    }
    err("can not tokenize: '%c'", *s);
    return 1;
  }

  tp("  ret\n");
  return 0;
}
