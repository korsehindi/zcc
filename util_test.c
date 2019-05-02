#include "zcc.h"

void expect(int line, int expected, int actual) {
  if (expected == actual)
    return;
  err("%d: %d expected, but got %d\n", line, expected, actual);
}

void test_vec() {
  Vec *v = new_vec();
  expect(__LINE__, 0, v->len);
  for (int i = 0; i < 100; i++)
    vec_push(v, (void *)(intptr_t)i);
  expect(__LINE__, 100, v->len);
  expect(__LINE__, 0, (intptr_t)v->data[0]);
  expect(__LINE__, 50, (intptr_t)v->data[50]);
  expect(__LINE__, 99, (intptr_t)v->data[99]);
}

void util_test() {
  test_vec();
  printf("OK\n");
}
