//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      isingb33.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#ifndef ASSIST3_H
#define ASSIST3_H

#include "smt_exp/smt_solver.hh"

#include <string>
#include <cstdlib>
#include <sstream>


class Assist3 : public SmtFunction {



public:
  Assist3(std::string name) : SmtFunction(name) {
    _index = _counter;
    ++_counter;
    std::stringstream ss;
    ss << _name << _index;
    _name = ss.str();
  }

  virtual ~Assist3();
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

class AssistX : public SmtFunction {

public:
  AssistX(std::string name, unsigned input_num, unsigned var_count) : 
    SmtFunction(name),
  _input_num(input_num),
  _var_counter(var_count) {}

  virtual ~AssistX();
  virtual std::string getFuncDecl() const;
  virtual std::string getFuncExpr(const CoverTable& cov) const;

  virtual void init();

private:
  void initInputs();
  void initExprs();

  unsigned _input_num;
  unsigned _var_counter;

  std::vector<SmtExpression*> _expressions;
  std::vector<SmtInput*> _inputs;
  std::string _return_type;



};





#endif

