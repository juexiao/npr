//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      smt_solver.cc
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#ifndef SMT_SOLVER_H
#define SMT_SOLVER_H

#include <string>
#include <unordered_set>
#include <vector>

class SmtVar;
class SmtFunction;


class CoverTable : public std::vector<int> {

public:
  CoverTable() {}
  CoverTable(const std::vector<int>& other) : 
    std::vector<int>(other) {}


  CoverTable(std::string input);
  std::string toString() const;


};

class SmtVar {

public:

  SmtVar(std::string data_type, double upper, double lower, std::string name) :
    _data_type(data_type),
    _upper_bound(upper),
    _lower_bound(lower),
    _name(name) {}

  std::string getType() const { return _data_type; }
  std::string getName() const { return _name;}

  double getUB() const { return _upper_bound; }
  double getLB() const { return _lower_bound; }

  std::string getExpr() const;


private:

    //smt data type
    std::string _data_type;

    //smt data range
    double _upper_bound;
    double _lower_bound;

    //smt data var name
    std::string _name;

};

class SmtInput {

public:
  SmtInput(std::string name, std::string type) :
    _name(name), _type(type) {}

  std::string getName() const { return _name; }
  std::string getType() const { return _type; }

private:
  std::string _name;
  std::string _type;

};

class SmtExpression {

public:
  SmtExpression(std::string oper, std::string var1, std::string var2, std::string var3) :
    _operator(oper),
    _var1(var1),
    _var2(var2),
    _var3(var3) {}

  std::string getExprTwo();
  std::string getExprThree();

private:
  std::string _operator;
  std::string _var1;
  std::string _var2;
  std::string _var3;

};

class SmtFunction {
public:
  SmtFunction (std::string name) : _name(name) {}
  virtual ~SmtFunction() {}

  std::string getName() const { return _name; }

  virtual void init() = 0;
  virtual std::string getFuncDecl() const = 0;
  virtual std::string getFuncExpr(const CoverTable& cov) const = 0;
protected:
  std::string _name;
};

class IsingFourFunc : public SmtFunction {

public:
  IsingFourFunc(std::string name) : SmtFunction(name) {}
  virtual ~IsingFourFunc();

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


class TruthTable {

public:
  typedef std::vector<CoverTable>::iterator covertable_iter;
  typedef std::vector<CoverTable>::const_iterator covertable_const_iter;

  void loadTruthTable(std::string file);

  covertable_const_iter begin() const { return _valid.begin(); }
  covertable_const_iter end() const { return _valid.end(); }

  size_t size() const { return _valid.size(); }

  bool isValid(std::string stri) const { return  _valid_str.count(stri); }

private:
  std::vector<CoverTable> _valid;
  std::unordered_set<std::string> _valid_str;

};

class SmtWriter {

public:
  SmtWriter() : _truth_table(NULL) {}
  ~SmtWriter();


  void initFunction(std::string filename);
  void writeSmt(std::string filename);

  
private:

  void initVars(unsigned var_num);
  void initFunctions();
  void initAsserts();

  std::string padStr(std::string orig, size_t n);

  std::vector<SmtVar*> _vars;
  std::vector<SmtFunction*> _funcs;
  std::vector<std::string> _asserts;
  TruthTable* _truth_table;

};


#endif

