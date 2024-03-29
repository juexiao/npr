//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      asssit3.cc
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================


#include "smt_exp/assist3.hh"
#include "smt_exp/bin_iterator.hh"

#include <cassert>

unsigned int Assist3::_counter = 0;

Assist3::~Assist3() {

  for (size_t i = 0; i < _expressions.size(); ++i) {
    delete _expressions[i];
  }
  _expressions.clear();

  for (size_t i =0; i < _inputs.size(); ++i) {
    delete _inputs[i];
  }
  _inputs.clear();

}

void Assist3::init() {
  _return_type = "Real";
  initInputs();
  initExprs();
}


void Assist3::initInputs() {
  for (int i = 1; i <= 3; ++i) {
    SmtInput* new_input = new SmtInput("q" + std::to_string(i), "Real");
    _inputs.push_back(new_input);
  }
}

void Assist3::initExprs() {

  BinIterator bin(3);
  int counter = 0;
  for (; !bin.end(); ++bin, ++counter) {
    std::vector<int> value = bin.value();
    std::vector<std::string> exps;
    for (unsigned long i = 0; i < 3; ++i) {
      assert(value[i] == 0 || value[i] == 1);
      std::string exp1 = value[i] == 1 ? "(/ (+ 1 " + _inputs[i]->getName() + ") 2)" :
                                         "(/ (- 1 " + _inputs[i]->getName() + ") 2)" ;
      exps.push_back(exp1);
    }

    SmtExpression* smt_exp = new SmtExpression("*", "c" + std::to_string(counter+1), exps[0], exps[1], exps[2]);
    _expressions.push_back(smt_exp);
  }

}

std::string Assist3::getFuncExpr(const CoverTable& cov) const {

  std::stringstream ss;
  ss << "(" << _name;
  assert(cov.size() == 3);
  for (size_t i = 0; i < cov.size(); ++i) {
    assert(cov[i]==1 || cov[i]==-1);
    ss << " " << cov[i];
  }
  ss << ")";

  return ss.str();

}

std::string Assist3::getFuncDecl() const {

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
  ss << "(+";
  for (unsigned long i = 0; i < _expressions.size(); ++i) {
    ss << " " << _expressions[i]->getExprFour();
  }
  ss << ")";

  //3) tail
  ss << ")" << std::endl;
  return ss.str();

}

AssistX::~AssistX() {
  for (size_t i = 0; i < _expressions.size(); ++i) {
    delete _expressions[i];
  }
  _expressions.clear();

  for (size_t i =0; i < _inputs.size(); ++i) {
    delete _inputs[i];
  }
  _inputs.clear();
}


void AssistX::init() {
  _return_type = "Real";
  initInputs();
  initExprs();
}

void AssistX::initInputs() {
  for (unsigned i = 0; i < _input_num; ++i) {
    SmtInput* new_input = new SmtInput("q" + std::to_string(i), "Real");
    _inputs.push_back(new_input);
  }
}

void AssistX::initExprs() {

  BinIterator bin(_input_num);
  unsigned counter = _var_counter;
  for (; !bin.end(); ++bin, ++counter) {
    std::vector<int> value = bin.value();
    std::vector<std::string> exps;
    exps.push_back("c" + std::to_string(counter));
    for (unsigned long i = 0; i < _input_num; ++i) {
      assert(value[i] == 0 || value[i] == 1);
      std::string exp1 = value[i] == 1 ? "(/ (+ 1 " + _inputs[i]->getName() + ") 2)" :
                                         "(/ (- 1 " + _inputs[i]->getName() + ") 2)" ;
      exps.push_back(exp1);
    }

    SmtExpression* smt_exp = new SmtExpression("*", exps);
    _expressions.push_back(smt_exp);
  }

}

std::string AssistX::getFuncExpr(const CoverTable& cov) const {

  std::stringstream ss;
  ss << "(" << _name;
  assert(cov.size() == _input_num);
  for (size_t i = 0; i < cov.size(); ++i) {
    assert(cov[i]==1 || cov[i]==-1);
    ss << " " << cov[i];
  }
  ss << ")";

  return ss.str();
}


std::string AssistX::getFuncDecl() const {

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
  ss << "(+";
  for (unsigned long i = 0; i < _expressions.size(); ++i) {
    ss << " " << _expressions[i]->getExprReal();
  }
  ss << ")";

  //3) tail
  ss << ")" << std::endl;
  return ss.str();

}






