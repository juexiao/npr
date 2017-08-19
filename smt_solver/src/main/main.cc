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
#include "smt_exp/smt_adv.hh"


void generate_k44_chimera() {

  // *generate k44 chimera
  TGT_Graph graph;
  graph.initChimeraGraph();
  graph.buildPtree();
  graph.writeJson("k44.chimera");
  //graph.loadJson("k44.chimera");
}

void generate_k33_chimera() {
  TGT_Graph graph;
  graph.initChimeraGraph33();
  graph.buildPtree();
  graph.writeJson("k33.chimera");

}

void test_all_2_input_on_k4_graph() {
  // *find all ising model for 2 input gate on k4 graph
  TruthTableGenerator generator(2);
  generator.generateAllTruthTable();
}


void test_one_truth_table_with_topo() {
  TruthTable truthtable;
  truthtable.loadTruthTable("and");
  TGT_Graph graph;
  //graph.loadJson("k44.chimera");
  graph.loadJson("k44.chimera");
  SmtAdvWriter smt_writer(&graph, &truthtable);
  smt_writer.initSmt();
  smt_writer.writeSmt("test.smt2");

}


int main(int argc, char** argv) {


  //generate_k33_chimera();
  test_one_truth_table_with_topo();
  //test_all_2_input_on_k4_graph(); 


  return 0;
}

