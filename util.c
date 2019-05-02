#include "zcc.h"

void err(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

Vec *new_vec() {
  Vec *v = malloc(sizeof(Vec));
  v->data = malloc(sizeof(void *) * 16);
  v->len = 0;
  v->cap = 16;
  return v;
}

void vec_push(Vec *v, void *x) {
  if (v->cap == v->len) {
    v->cap *= 2;
    v->data = realloc(v->data, sizeof(void *) * v->cap);
  }
  v->data[v->len++] = x;
}
