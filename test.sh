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

try 0 "0;"
try 123 "123;"
try 3 "1+2;"
try 0 "11 + 22 - 33;"
try 7 "1+2*3;"
try 1 "4/2-1;"
try 4 "8/4+2*1;"
try 3 "3*(2-1);"
try 6 "(8+4)/2;"
try 1 "-1+2;"
try 1 "-1*+1/-1;"
try 1 "0==0;"
try 0 "0==1;"
try 0 "0!=0;"
try 1 "0!=1;"
try 1 "0<=0;"
try 0 "1<=0;"
try 1 "0>=0;"
try 0 "0>=1;"
try 0 "0<0;"
try 1 "0<1;"
try 0 "0>0;"
try 1 "1>0;"
try 1 "a=1;"

echo OK
