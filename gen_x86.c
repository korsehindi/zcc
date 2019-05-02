#include "zcc.h"

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

static void gen_code(Node *n) {
  if (n->ty == ND_NUM) {
    tp("push %d", n->val);
    return;
  }

  gen_code(n->lhs);
  gen_code(n->rhs);
  tp("pop rdi");
  tp("pop rax");

  switch (n->ty) {
  case '+':
    tp("add rax, rdi");
    break;
  case '-':
    tp("sub rax, rdi");
    break;
  case '*':
    tp("mul rdi");
    break;
  case '/':
    tp("mov rdx, 0");
    tp("div rdi");
    break;
  case ND_EQ:
    tp("cmp rdi, rax");
    tp("sete al");
    tp("movzx rax, al");
    break;
  case ND_NE:
    tp("cmp rdi, rax");
    tp("setne al");
    tp("movzx rax, al");
    break;
  case ND_L:
    tp("cmp rdi, rax");
    tp("setl al");
    tp("movzx rax, al");
    break;
  case ND_LE:
    tp("cmp rdi, rax");
    tp("setle al");
    tp("movzx rax, al");
    break;
  }

  tp("push rax");
}

void gen_x86(Vec *prog) {
  p(".intel_syntax noprefix");

#ifdef __APPLE__
  p(".global _main");
  p("_main:");
#else
  p(".global main");
  p("main:");
#endif

  tp("push rbp");
  tp("mov rbp, rsp");
  tp("sub rsp, 208");

  for (int i = 0; prog->data[i]; i++) {
    gen_code(prog->data[i]);
    tp("pop rax");
  }

  tp("add rsp, 208");
  tp("mov rsp, rbp");
  tp("pop rbp");

  tp("ret");
}