//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      tgt_ising.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#ifndef TGT_ISING_H
#define TGT_ISING_H


#include "smt_exp/smt_solver.hh"
#include "hw_tgt/tgt_writer.hh"
class AssistX;

class TGT_Ising : public SmtFunction {
public:

  enum InputType {VAR, AUX};


public:
  //TGT_Ising(std::string topofile, std::string name);
  TGT_Ising(TGT_Graph* graph, std::string name);
  virtual ~TGT_Ising();

  virtual std::string getFuncDecl() const;
  virtual std::string getFuncExpr(const CoverTable& cov) const;
  std::string getFuncExprAssist(const CoverTable& cov) const;

  virtual void init();

  TGT_Graph* getTGTGraph() const { return _tgt_graph; }


  unsigned getAssistNum() const;
  unsigned getNonAssistNum() const;

  std::vector<SmtVar*> getVarVec();
  std::vector<SmtVar*> getBinVarVec();

  std::vector<int> getAssign() { return _input_assign; }
  std::vector<AssistX*> getAssistFunctions();

private:
  void initInputs();
  void initInputInfo();
  void initVars();
  void initAssitFunc();
  void initExprs();


  void graphCheck();

  TGT_Graph*  _tgt_graph;

  std::vector<SmtExpression*> _expressions;
  std::vector<SmtInput*> _inputs;

  std::map<std::string, SmtVar*> _name_to_var;
  std::map<std::string, SmtVar*> _name_to_bin_var;

  std::string _return_type;
  std::vector<int> _input_assign;

  std::map<unsigned, AssistX*> _index_to_func;


};


#endif


