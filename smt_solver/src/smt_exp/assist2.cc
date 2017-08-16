//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      smt_solver.cc
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#include "smt_exp/assist2.hh"

#include <cassert>


Assist2::~Assist2() {

  for (size_t i = 0; i < _expressions.size(); ++i) {
    delete _expressions[i];
  }
  _expressions.clear();

  for (size_t i =0; i < _inputs.size(); ++i) {
    delete _inputs[i];
  }
  _inputs.clear();

}

void Assist2::init() {

  _return_type = "Real";

  initInputs();
  initExprs();

}


void Assist2::initInputs() {
  for (int i = 1; i <= 2; ++i) {
    SmtInput* new_input = new SmtInput("q" + std::to_string(i), "Real");
    _inputs.push_back(new_input);
  }
}

void Assist2::initExprs() {

  {
    std::string var1 = "c1";
    std::string var2 = "c2";
    std::string var3 = "c3";
    std::string var4 = "c4";
    SmtExpression* smt_exp = new SmtExpression("+", var1, var2, var3, var4);
    _expressions.push_back(smt_exp);
  }


  {
    std::string var1 = "c1";
    std::string var2 = "(- 0 c2)";
    std::string var3 = "c3";
    std::string var4 = "(- 0 c4)";
    SmtExpression* smt_exp = new SmtExpression("+", var1, var2, var3, var4);
    _expressions.push_back(smt_exp);
  }


  {
    std::string var1 = "c1";
    std::string var2 = "c2";
    std::string var3 = "(- 0 c3)";
    std::string var4 = "(- 0 c4)";
    SmtExpression* smt_exp = new SmtExpression("+", var1, var2, var3, var4);
    _expressions.push_back(smt_exp);
  }


  {
    std::string var1 = "c1";
    std::string var2 = "(- 0 c2)";
    std::string var3 = "(- 0 c3)";
    std::string var4 = "c4";
    SmtExpression* smt_exp = new SmtExpression("+", var1, var2, var3, var4);
    _expressions.push_back(smt_exp);
  }

  {
    SmtExpression* smt_exp = new SmtExpression(
        "*", 
        _expressions[1]->getExprFour(),
        _inputs[0]->getName(),
        ""
        );
    _expressions.push_back(smt_exp);
  }


  {
    SmtExpression* smt_exp = new SmtExpression(
        "*", 
        _expressions[2]->getExprFour(),
        _inputs[1]->getName(),
        ""
        );
    _expressions.push_back(smt_exp);
  }

  {
    SmtExpression* smt_exp = new SmtExpression(
        "*", 
        _expressions[3]->getExprFour(),
        _inputs[0]->getName(),
        _inputs[1]->getName()
        );
    _expressions.push_back(smt_exp);
  }

}

std::string Assist2::getFuncDecl() const {

  //1) head
  std::stringstream ss;
  ss << "(define-fun ";
  ss << _name;
  ss << "(";
  for (size_t i = 0; i < _inputs.size(); ++i)
    ss << "(" << _inputs[i]->getName() << " " << _inputs[i]->getType() << ")";
  ss << ")";
  ss << _return_type << std::endl;

  //2) body
  ss << "(* 0.25 ";
  ss << "(+";
  ss << " " << _expressions[0]->getExprThree();
  ss << " " << _expressions[4]->getExprThree();
  ss << " " << _expressions[5]->getExprThree();
  ss << " " << _expressions[6]->getExprThree();
  ss << ") )" << std::endl;


  //3) tail
  ss << ")" << std::endl;
  return ss.str();

}

std::string Assist2::getFuncExpr(const CoverTable& cov) const {

  std::stringstream ss;
  ss << "(" << _name;
  assert(cov.size() == 3);
  for (size_t i = 0; i < cov.size()-1; ++i) {
    assert(cov[i]==1 || cov[i]==-1);
    ss << " " << cov[i];
  }
  ss << ")";

  return ss.str();
}



