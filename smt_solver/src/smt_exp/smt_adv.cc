//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      smt_adv.cc
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#include "smt_exp/smt_adv.hh"
#include "smt_exp/tgt_ising.hh"
#include "smt_exp/bin_iterator.hh"
#include "smt_exp/assist3.hh"

void SmtAdvWriter::initSmt() {
  _func = new TGT_Ising(_graph, "chimeraIsing");
  unsigned real_var_num = (unsigned)_truth_table->getInputNum() + 1;
  _func->init();

  assert(_func->getNonAssistNum() == real_var_num);


  initVars();

  initAsserts();




}

SmtAdvWriter::~SmtAdvWriter() {
  for (size_t i = 0; i < _bin_vars.size(); ++i)
    delete _bin_vars[i];

  _bin_vars.clear();

  if (_func) {
    delete _func;
    _func = NULL;
  }

}


void SmtAdvWriter::initAsserts() {
  //1) all valid 
  {
    std::stringstream ss;
    ss << "(assert (=";
    TruthTable::covertable_const_iter c_iter = _truth_table->begin();
    for (; c_iter != _truth_table->end(); ++c_iter) {
      ss << " " << _func->getFuncExprAssist(*c_iter);
    }
    ss << ") )" << std::endl;
    _asserts.push_back(ss.str());
  }

  {
    std::stringstream ss;
    ss << "(assert (= g ";
    TruthTable::covertable_const_iter c_iter = _truth_table->begin();
    ss <<  _func->getFuncExprAssist(*c_iter);
    ss << ") )" << std::endl;
    _asserts.push_back(ss.str());
  }

  //2) partial valid
  {
    std::vector<int> _input_assign = _func->getAssign();
    unsigned total_var = _func->getAssistNum() + _func->getNonAssistNum();
    BinIterator bin_iter(total_var);
    for (; !bin_iter.end(); ++bin_iter) {
      std::vector<int> vals = bin_iter.value();
      assert(vals.size() == _input_assign.size());
      std::vector<int> new_vals;
      new_vals.resize(_func->getNonAssistNum());
      for (size_t i = 0; i < vals.size(); ++i) {
        if (_input_assign[i] != -1)
          new_vals[(unsigned)_input_assign[i]] = vals[i];
      }
      CoverTable cov(new_vals);
      CoverTable cov_old(vals);
      if (!_truth_table->isValid(cov.toString())) continue; 
      
      std::stringstream ss;
      ss << "(assert (<=";
      ss << " g";
      ss << " " << _func->getFuncExpr(cov_old);
      ss << ") )" << std::endl;
      _asserts.push_back(ss.str());
    }
  }

  //3) non valid
  {
    std::vector<int> _input_assign = _func->getAssign();
    unsigned total_var = _func->getAssistNum() + _func->getNonAssistNum();
    BinIterator bin_iter(total_var);
    for (; !bin_iter.end(); ++bin_iter) {
      std::vector<int> vals = bin_iter.value();
      assert(vals.size() == _input_assign.size());
      std::vector<int> new_vals;
      new_vals.resize(_func->getNonAssistNum());
      for (size_t i = 0; i < vals.size(); ++i) {
        if (_input_assign[i] != -1)
          new_vals[(unsigned)_input_assign[i]] = vals[i];
      }
      CoverTable cov(new_vals);
      CoverTable cov_old(vals);
      if (_truth_table->isValid(cov.toString())) continue; 

      std::stringstream ss;
      ss << "(assert (<";
      ss << " k"; 
      ss << " " << _func->getFuncExpr(cov_old);
      ss << ") )" << std::endl;
      _asserts.push_back(ss.str());
    }

  }

  {
    std::stringstream ss;
    ss << "(assert (< 0 (- k g) ) )" << std::endl;
    ss << "(check-sat)" << std::endl;
    _asserts.push_back(ss.str());
  }



}

void SmtAdvWriter::initVars() {
  std::vector<SmtVar*> vars = _func->getVarVec();
  for (size_t i = 0; i < vars.size(); ++i)
    _vars.push_back(vars[i]);

  _bin_vars = _func->getBinVarVec();

  {
    SmtVarUn* smt_var = new SmtVarUn("Real", "g");
    _vars.push_back(smt_var);
  }

  {
    SmtVarUn* smt_var = new SmtVarUn("Real", "k");
    _vars.push_back(smt_var);
  }

}

void SmtAdvWriter::writeSmt(std::string filename) {
  std::ofstream outfile;
  outfile.open(filename.c_str());

  std::vector<SmtVarBase*>::iterator var_iter = _vars.begin();
  for (; var_iter != _vars.end(); ++var_iter) {
    outfile << (*var_iter)->getExpr();
  }
  outfile << std::endl;

  std::vector<SmtVar*>::iterator var_b_iter = _bin_vars.begin();
  for (; var_b_iter != _bin_vars.end(); ++var_b_iter) {
    outfile << (*var_b_iter)->getExpr2();
  }
  outfile << std::endl;

  std::vector<AssistX*> funcs = _func->getAssistFunctions();
  for (size_t i = 0; i < funcs.size(); ++i)
    outfile << funcs[i]->getFuncDecl() << std::endl;

  outfile << _func->getFuncDecl() << std::endl;


  std::vector<std::string>::iterator assert_iter = _asserts.begin();
  for (; assert_iter != _asserts.end(); ++assert_iter) {
    outfile << *assert_iter ;
  }
  outfile << std::endl;
  outfile.close();


}


