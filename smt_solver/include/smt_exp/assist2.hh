//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      isingb33.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#ifndef ASSIST2_H
#define ASSIST2_H

#include "smt_exp/smt_solver.hh"

#include <string>
#include <cstdlib>
#include <sstream>

class Assist2 : public SmtFunction {

public:
  Assist2(std::string name) : SmtFunction(name) {
    _index = _counter;
    ++_counter;
    std::stringstream ss;
    ss << _name << _index;
    _name = ss.str();
  }

  virtual ~Assist2();
  virtual std::string getFuncDecl() const;
  virtual std::string getFuncExpr(const CoverTable& cov) const;

  virtual void init();


private:

  void initInputs();
  void initExprs();

  static unsigned int _counter;
  unsigned int _index;

  std::vector<SmtExpression*> _expressions;
  std::vector<SmtInput*> _inputs;
  std::string _return_type;

};



#endif


