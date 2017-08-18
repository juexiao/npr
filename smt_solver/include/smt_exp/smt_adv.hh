//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      smt_adv.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#ifndef SMT_ADV_H
#define SMT_ADV_H

#include "smt_solver.hh"


class TGT_Graph;
class TGT_Ising;

class SmtAdvWriter : public SmtWriterBase {

public:
  SmtAdvWriter(TGT_Graph* graph, TruthTable* truthtable) :
    SmtWriterBase(truthtable),
    _graph(graph) {}

  virtual ~SmtAdvWriter();

  void initSmt();
  void writeSmt(std::string filename);
 

private:
  TGT_Graph* _graph;
  TGT_Ising* _func;

  void initVars();

  void initAsserts();

  std::vector<SmtVar*> _bin_vars;

};


#endif



