#!/bin/bash
#

set +x
export PATH=/usr/local/bin:$PATH
bison -d sql_parser.y
flex -o sql_parser.lex.c sql_parser.l sql_parser.tab.h
