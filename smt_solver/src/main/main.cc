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
#include "smt_exp/truth_table_gen.hh"
#include "hw_tgt/tgt_writer.hh"



int main(int argc, char** argv) {


  TGT_Graph graph;
  graph.initChimeraGraph();
  graph.buildPtree();
  graph.writeJson("k44.chimera");
  
  //TruthTableGenerator generator(2);
  //generator.generateAllTruthTable();





  return 0;
}

