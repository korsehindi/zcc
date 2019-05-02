#!/bin/bash

try() {
  expected="$1"
  input="$2"

  ./zcc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$expected expected, but got $actual"
    exit 1
  fi
}

try 0 "return 0;"
try 123 "return 123;"
try 3 "return 1+2;"
try 0 "return 11 + 22 - 33;"
try 7 "return 1+2*3;"
try 1 "return 4/2-1;"
try 4 "return 8/4+2*1;"
try 3 "return 3*(2-1);"
try 6 "return (8+4)/2;"
try 1 "return -1+2;"
try 1 "return -1*+1/-1;"
try 1 "return 0==0;"
try 0 "return 0==1;"
try 0 "return 0!=0;"
try 1 "return 0!=1;"
try 1 "return 0<=0;"
try 0 "return 1<=0;"
try 1 "return 0>=0;"
try 0 "return 0>=1;"
try 0 "return 0<0;"
try 1 "return 0<1;"
try 0 "return 0>0;"
try 1 "return 1>0;"
try 1 "return a=1;"
try 3 "a=1; b=2; return a+b;"
try 6 "a=1; b=2*3+4; return a+b/2;"

echo OK
