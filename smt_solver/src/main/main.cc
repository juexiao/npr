//============================================================================
// Copyright (c) 2015, All Right Reserved, UCLA EDA LAB
//
// file:      main.cc
// author:    Juexiao Su 
// purpose:   main()
// revision history:
//============================================================================

#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <sys/resource.h>
#include <cstdio>
#include <iostream>
#include <string>
#include "smt_exp/smt_solver.hh"



int main(int argc, char** argv) {
  if (argc != 2)
    std::cout << "Usage: smt_solver <lut file>" << std::endl;


  std::string filename(argv[1]);
  std::string outfile = "smt" + filename;

  SmtWriter smt_writer;
  smt_writer.initFunction(filename);
  smt_writer.writeSmt(outfile);


  return 0;
}

