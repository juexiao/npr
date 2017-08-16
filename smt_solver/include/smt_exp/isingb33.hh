//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      isingb33.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================


#ifndef ISINGB33_H
#define ISINGB33_H

#include "smt_exp/smt_solver.hh"
#include <string>

/* Ising funciton for B3,3 topology
 *
 *    x(1) - x(4)
 *        \ /
 *    x(2) - x(5)
 *        / \
 *    x(3) - x(6)
 *
 */


class IsingB33 : public SmtFunction {


public:
  IsingB33(std::string name) : SmtFunction(name) {}
  virtual ~IsingB33();

  virtual std::string getFuncDecl() const;
  virtual std::string getFuncExpr(const CoverTable& cov) const;

  virtual void init();

private:

  void initInputs();
  void initExprs();

  std::vector<SmtExpression*> _expressions;
  std::vector<SmtInput*> _inputs;
  std::string _return_type;

};



#endif


