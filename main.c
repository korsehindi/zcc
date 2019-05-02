#include "zcc.h"

int main(int argc, char **argv) {
  if (argc != 2)
    err("Usage: zcc <code>");

  if (argc == 2 && !strcmp(argv[1], "-test")) {
    util_test();
    return 0;
  }

  Vec *tokens = tokenize(argv[1]);
  Vec *prog = parse(tokens);
  gen_x86(prog);
  return 0;
}
