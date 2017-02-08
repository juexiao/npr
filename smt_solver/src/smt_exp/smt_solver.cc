//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      smt_solver.cc
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================


#include <sstream>
#include <fstream>
#include <cassert>

#include "smt_exp/smt_solver.hh"

#define BUF_SIZE 256

#define DASSERT(X) assert(X)


CoverTable::CoverTable(std::string input) {

  for (size_t i = 0; i < input.size(); ++i) {
    if (input[i] == '1')
      push_back(1);
    else if (input[i] == '0')
      push_back(-1);
    else
      DASSERT(0);
  }
    

}

std::string CoverTable::toString() const {

  std::stringstream ss;

  std::vector<int>::const_iterator it;
  for (it = begin(); it != end(); ++it)
    ss << *it;

  return ss.str();
}


std::string SmtVar::getExpr() const {

  std::stringstream ss;
  //1) head
  ss << "(declare-const " << _name << " " << _data_type << ")" << std::endl;

  //2) range
  ss << "(assert (<= " << _name << " " << _upper_bound << ")" << std::endl;
  ss << "(assert (>= " << _name << " " << _lower_bound << ")" << std::endl;

  return ss.str();
}

std::string SmtExpression::getExprTwo() {
  return "(" + _operator + " " + _var1 + " " + _var2 + ")";
}

std::string SmtExpression::getExprThree() {
  return "(" + _operator + " " + _var1 + " " + _var2 + " " + _var3 + ")";
}


IsingFourFunc::~IsingFourFunc() {

  for (size_t i = 0; i < _expressions.size(); ++i) {
    delete _expressions[i];
  }
  _expressions.clear();

  for (size_t i =0; i < _inputs.size(); ++i) {
    delete _inputs[i];
  }
  _inputs.clear();


}

void IsingFourFunc::init() {

  _return_type = "Real";

  initInputs();

  initExprs();

}

void IsingFourFunc::initInputs() {
  for (int i = 1; i <= 4; ++i) {
    SmtInput* new_input = new SmtInput("q" + std::to_string(i), "Real");
    _inputs.push_back(new_input);
  }
}

void IsingFourFunc::initExprs() {

  for (size_t i = 1; i <= 4; ++i) {
    std::string var1 = "a" + std::to_string(i);
    std::string var2 = _inputs[i]->getName();
    SmtExpression* smt_exp = new SmtExpression("*", var1, var2, "");
    _expressions.push_back(smt_exp);
  }

  for (size_t  i = 1; i <= 4; ++i) {
    for (size_t j = i + 1; j <= 4; ++j) {
      std::string var1 = "b" + std::to_string(i) + std::to_string(j);
      std::string var2 = _inputs[i]->getName();
      std::string var3 = _inputs[j]->getName();
      SmtExpression* smt_exp = new SmtExpression("*", var1, var2, var3);
      _expressions.push_back(smt_exp);
    }
  }

}


std::string IsingFourFunc::getFuncDecl() const {
  std::stringstream ss;
  //1) head
  ss << "(define-fun ";
  ss << _name;
  ss << "(";
  for (size_t i = 0; i < _inputs.size(); ++i)
    ss << "(" << _inputs[i]->getName() << " " << _inputs[i]->getType() << ")";
  ss << ")";
  ss << _return_type << std::endl;

  //2) main body
  ss << "(+";
  for (size_t i = 0; i < 4; ++i)
    ss << " " << _expressions[i]->getExprTwo(); 
  for (size_t i = 4; i < _expressions.size(); ++i)
    ss << " " << _expressions[i]->getExprThree(); 
  ss << ")" << std::endl;

  //3) tail
  ss << ")" << std::endl;

  return ss.str();
}

std::string IsingFourFunc::getFuncExpr(CoverTable cov) const {

  std::stringstream ss;
  ss << "(" << _name;
  DASSERT(cov.size() == 3);
  for (size_t i = 0; i < cov.size(); ++i) {
    DASSERT(cov[i]==1 || cov[i]==-1);
    ss << " " << cov[i];
  }
  ss << ")";

  return ss.str();

}


void TruthTable::loadTruthTable(std::string file) {
  std::ifstream infile;
  char buf[BUF_SIZE];
  infile.open(file.c_str());

  while (infile.getline(buf, BUF_SIZE)) {

    CoverTable cov;
    int i;
    for (i = 0; i < BUF_SIZE; ++i) {

      if (buf[i] == '1')
        cov.push_back(1);
      else if (buf[i] == '0')
        cov.push_back(-1);
      else
        DASSERT(0);
    }
    _valid_str.insert(cov.toString());

  }

  infile.close();
}

SmtWriter::~SmtWriter() {
  for (size_t i = 0; i < _vars.size(); ++i)
    delete _vars[i];

  for (size_t i = 0; i < _funcs.size(); ++i)
    delete _funcs[i];

  delete _truth_table;
}

void SmtWriter::initFunction(std::string filename) {

  _truth_table = new TruthTable;
  _truth_table->loadTruthTable(filename);

  initVars();

  SmtFunction* func = new IsingFourFunc("ising4");
  func->init();
  _funcs.push_back(func);


  initAsserts();

}

void SmtWriter::initVars() {

  for (size_t i = 1; i <= 4; ++i) {
    std::string var = "a" + std::to_string(i);
    SmtVar* smt_var = new SmtVar("Real", 2.0, -2.0, var);
    _vars.push_back(smt_var);
  }

  for (size_t i = 1; i <= 4; ++i) {
    for (size_t j = i+1; j <= 4; ++j) {
      std::string var = "b" + std::to_string(i) + std::to_string(j);
      SmtVar* smt_var = new SmtVar("Real", 1.0, -1.0, var);
      _vars.push_back(smt_var);
    }
  }

}

void SmtWriter::initAsserts() {

  //1) all valid 
  std::stringstream ss;
  ss << "(assert (=";
  TruthTable::covertable_const_iter c_iter = _truth_table->begin();
  for (; c_iter != _truth_table->end(); ++c_iter) {
    ss << " " <<  _funcs[0]->getFuncExpr(*c_iter);
  }
  ss << ") )" << std::endl;
  _asserts.push_back(ss.str());


  //2) non-valid cover tables


}






