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
  CoverTable(const std::vector<int>& other);


  CoverTable(std::string input);
  std::string toString() const;



};

class SmtVarBase {

public:

  SmtVarBase(std::string data_type, std::string name) :
    _data_type(data_type),
    _name(name) {}

  virtual ~SmtVarBase() {}


  std::string getType() const { return _data_type; }
  std::string getName() const { return _name; }

  virtual std::string getExpr() const = 0;

protected:
  std::string _name;
  std::string _data_type;

};

class SmtVar : public SmtVarBase {

public:

  SmtVar(std::string data_type, double upper, double lower, std::string name) :
    SmtVarBase(data_type, name),
    _upper_bound(upper),
    _lower_bound(lower)
  {}


  double getUB() const { return _upper_bound; }
  double getLB() const { return _lower_bound; }

  virtual std::string getExpr() const;
  std::string getExpr2() const;


private:


    //smt data range
    double _upper_bound;
    double _lower_bound;
};

class SmtVarUn : public SmtVarBase {

public:
  SmtVarUn(std::string data_type, std::string name) :
    SmtVarBase(data_type, name) {}

  virtual std::string getExpr() const;

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


  SmtExpression(std::string oper, std::string var1, std::string var2, std::string var3, std::string var4) :
    _operator(oper),
    _var1(var1),
    _var2(var2),
    _var3(var3),
    _var4(var4) {}

  std::string getExprTwo();
  std::string getExprThree();
  std::string getExprFour();

  std::string getExpr();

private:
  std::string _operator;
  std::string _var1;
  std::string _var2;
  std::string _var3;
  std::string _var4;

};

class SmtFunction {
public:
  SmtFunction (std::string name) : _name(name) {}
  virtual ~SmtFunction() {}

  std::string getName() const { return _name; }

  virtual void init() = 0;
  virtual std::string getFuncDecl() const = 0;
  virtual std::string getFuncExpr(const CoverTable& cov) const = 0;

  virtual std::string getFuncExpr(const CoverTable& cov, std::string expr) const;
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

  TruthTable() : _input_num(0) {}

  typedef std::vector<CoverTable>::iterator covertable_iter;
  typedef std::vector<CoverTable>::const_iterator covertable_const_iter;

  void loadTruthTable(std::string file);

  covertable_const_iter begin() const { return _valid.begin(); }
  covertable_const_iter end() const { return _valid.end(); }

  size_t size() const { return _valid.size(); }

  bool isValid(std::string stri) const { return  _valid_str.count(stri); }

  int getInputNum() const { return _input_num; }

private:
  std::vector<CoverTable> _valid;
  std::unordered_set<std::string> _valid_str;

  void sanityCheck();

  int _input_num;

};

class SmtWriterBase {

public:
  //SmtWriterBase() : _truth_table(NULL) {}

  SmtWriterBase(TruthTable* truth_table) :
    _truth_table(truth_table) {}

  virtual ~SmtWriterBase();


protected:
  std::string padStr(std::string orig, size_t n);
  std::vector<SmtVarBase*> _vars;
  std::vector<SmtFunction*> _funcs;
  std::vector<std::string> _asserts;
  TruthTable* _truth_table;


};

class SmtWriter : public SmtWriterBase {

public:
  SmtWriter() : SmtWriterBase(NULL) {}
  virtual ~SmtWriter();


  virtual void initFunction(std::string filename);
  void initFunction2(std::string filename);
  virtual void writeSmt(std::string filename);
  virtual void initAsserts();

private:

  void initVars(unsigned var_num);
  void initBinVars(unsigned var_num);
  void initAsserts2();


  std::vector<SmtVar*> _bin_vars;

};


class SmtWriterAdv : public SmtWriterBase {

public:
  SmtWriterAdv(std::string topo_file, TruthTable* truth_table) :
    SmtWriterBase(truth_table), _topo_file(topo_file) {}

  void initSmt();
  virtual void writeSmt(std::string filename);

private:

  std::string _topo_file;

};


#endif

