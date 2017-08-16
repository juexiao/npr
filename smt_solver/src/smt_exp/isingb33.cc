//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      smt_solver.cc
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================



#include "smt_exp/isingb33.hh"

#include <sstream>
#include <cassert>


#define DASSERT(X) assert(X)

IsingB33::~IsingB33() {

  for (size_t i = 0; i < _expressions.size(); ++i) {
    delete _expressions[i];
  }
  _expressions.clear();

  for (size_t i =0; i < _inputs.size(); ++i) {
    delete _inputs[i];
  }
  _inputs.clear();

}

void IsingB33::init() {

  _return_type = "Real";

  initInputs();
  initExprs();

}

void IsingB33::initInputs() {
  for (int i = 1; i <= 6; ++i) {
    SmtInput* new_input = new SmtInput("q" + std::to_string(i), "Real");
    _inputs.push_back(new_input);
  }
}

void IsingB33::initExprs() {

  for (size_t i = 1; i <= 6; ++i) {
    std::string var1 = "a" + std::to_string(i);
    std::string var2 = _inputs[i-1]->getName();
    SmtExpression* smt_exp = new SmtExpression("*", var1, var2, "");
    _expressions.push_back(smt_exp);
  }


  for (size_t  i = 1; i <= 3; ++i) {
    for (size_t j = 4; j <= 6; ++j) {
      std::string var1 = "b" + std::to_string(i) + std::to_string(j);
      std::string var2 = _inputs[i-1]->getName();
      std::string var3 = _inputs[j-1]->getName();
      SmtExpression* smt_exp = new SmtExpression("*", var1, var2, var3);
      _expressions.push_back(smt_exp);
    }
  }

}

std::string IsingB33::getFuncDecl() const {
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


std::string IsingB33::getFuncExpr(const CoverTable& cov) const {

  std::stringstream ss;
  ss << "(" << _name;
  DASSERT(cov.size() == 6);
  for (size_t i = 0; i < cov.size()-1; ++i) {
    DASSERT(cov[i]==1 || cov[i]==-1);
    ss << " " << cov[i];
  }
  ss << " " << cov[cov.size()-1];
  ss << ")";

  return ss.str();
}






