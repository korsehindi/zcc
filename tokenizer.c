#include "zcc.h"

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

static int is_alnum(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
         ('0' <= c && c <= '9') || (c == '_');
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
    if (strncmp(s, "return", 6) == 0 && !is_alnum(s[6])) {
      add_token(v, TK_RETURN, s);
      s += 6;
      continue;
    }
    if (strchr("+-*/()=;", *s)) {
      add_token(v, *s, s);
      s++;
      continue;
    }
    if (isalpha(*s)) {
      add_token(v, TK_IDENT, s);
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
