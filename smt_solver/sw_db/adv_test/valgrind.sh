#!/bin/bash

valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ../../exe/smt-solver-debug
