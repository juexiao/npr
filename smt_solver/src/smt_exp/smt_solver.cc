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
#include <algorithm>

#include "smt_exp/smt_solver.hh"
#include "smt_exp/bin_iterator.hh"
#include "smt_exp/assist2.hh"
#include "smt_exp/assist3.hh"

#define BUF_SIZE 256

#define DASSERT(X) assert(X)

std::string SmtFunction::getFuncExpr(const CoverTable& cov, std::string expr) const {
  DASSERT(cov.size() == 3);
  std::stringstream ss;
  ss << "(" << _name;

  for (size_t i = 0; i < cov.size()-1; ++i) {
    DASSERT(cov[i]==1 || cov[i]==-1);
    ss << " " << cov[i];
  }
  ss << " " << cov[cov.size()-1];
  ss << " " << expr;
  ss << ")";


  return ss.str();
}


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

CoverTable::CoverTable(const std::vector<int>& other) :
  std::vector<int>(other) {
  for (size_t i = 0; i < size(); ++i) {
    DASSERT(at(i) == 1 || at(i) == 0);
    if (at(i) == 0) at(i) = -1;
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
  ss << "(assert (<= " << _name << " " << _upper_bound << "))" << std::endl;
  ss << "(assert (>= " << _name << " " << _lower_bound << "))" << std::endl;

  return ss.str();
}

std::string SmtVar::getExpr2() const {

  std::stringstream ss;
  //1) head
  ss << "(declare-const " << _name << " " << _data_type << ")" << std::endl;

  //2) range
  ss << "(assert (or (= " << _name << " " << (int)_upper_bound << ") (= " << _name << " " << (int)_lower_bound  << ") ) )" << std::endl;

  return ss.str();
}

std::string SmtVarUn::getExpr() const {
  std::stringstream ss;
  ss << "(declare-const " << _name << " " << _data_type << ")" << std::endl;
  return ss.str();
}


std::string SmtExpression::getExprTwo() {
  return "(" + _operator + " " + _var1 + " " + _var2 + ")";
}

std::string SmtExpression::getExprThree() {
  return "(" + _operator + " " + _var1 + " " + _var2 + " " + _var3 + ")";
}


std::string SmtExpression::getExprFour() {
  return "(" + _operator + " " + _var1 + " " + _var2 + " " + _var3 + " " + _var4 + ")";
}

std::string SmtExpression::getExpr() {
  if (_var3 == "" && _var4 == "")
    return getExprTwo();
  else if (_var4 == "")
    return getExprThree();
  assert(0);
  return "";
}

std::string SmtExpression::getExprReal() {
  std::stringstream ss;
  ss << "(" << _operator;
  for (size_t i = 0; i < _vars.size(); ++i)
    ss << " " << _vars[i];
  ss << ") ";
  return ss.str();
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
    std::string var2 = _inputs[i-1]->getName();
    SmtExpression* smt_exp = new SmtExpression("*", var1, var2, "");
    _expressions.push_back(smt_exp);
  }

  for (size_t  i = 1; i <= 4; ++i) {
    for (size_t j = i + 1; j <= 4; ++j) {
      std::string var1 = "b" + std::to_string(i) + std::to_string(j);
      std::string var2 = _inputs[i-1]->getName();
      std::string var3 = _inputs[j-1]->getName();
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

std::string IsingFourFunc::getFuncExpr(const CoverTable& cov) const {

  std::stringstream ss;
  ss << "(" << _name;
  DASSERT(cov.size() == 4);
  for (size_t i = 0; i < cov.size()-1; ++i) {
    DASSERT(cov[i]==1 || cov[i]==-1);
    ss << " " << cov[i];
  }
  ss << " " << cov[cov.size()-1];
  ss << ")";

  return ss.str();

}

void TruthTable::sanityCheck() {
  _input_num = 0;
  if (_valid.size() == 0) return;

  _input_num = (int)_valid[0].size()-1;

  for (size_t i = 0; i < _valid.size(); ++i)
    assert(_input_num == (int)_valid[i].size()-1);
}

void TruthTable::loadTruthTable(std::string file) {
  std::ifstream infile;
  char buf[BUF_SIZE];
  infile.open(file.c_str());

  while (infile.getline(buf, BUF_SIZE, '\n')) {
    std::string str_buf(buf);
    //std::cout << str_buf << std::endl;

    CoverTable cov;
    size_t i;
    for (i = 0; i < str_buf.size(); ++i) {

      if (buf[i] == '1')
        cov.push_back(1);
      else if (buf[i] == '0')
        cov.push_back(-1);
      else if (buf[i] == 0)
        break;
      else
        DASSERT(0);
    }
    _valid.push_back(cov);
    _valid_str.insert(cov.toString());

  }

  infile.close();
  sanityCheck();
}

SmtWriter::~SmtWriter() {
  for (size_t i = 0; i < _bin_vars.size(); ++i)
    delete _bin_vars[i];

  _bin_vars.clear();

}

void SmtWriter::initFunction(std::string filename) {

  _truth_table = new TruthTable;
  _truth_table->loadTruthTable(filename);

  initVars(4);

  SmtFunction* func = new IsingFourFunc("ising4");
  func->init();
  _funcs.push_back(func);


  initAsserts();

}

void SmtWriter::initFunction2(std::string filename) {

  _truth_table = new TruthTable;
  _truth_table->loadTruthTable(filename);

  initVars(4);
  initBinVars(8);

  SmtFunction* func = new IsingFourFunc("ising4");
  func->init();
  _funcs.push_back(func);

  SmtFunction* assst = new Assist3("assist");
  assst->init();
  _funcs.push_back(assst);


  initAsserts2();

}



void SmtWriter::initVars(unsigned var_num) {

  for (size_t i = 1; i <= var_num; ++i) {
    std::string var = "a" + std::to_string(i);
    SmtVar* smt_var = new SmtVar("Real", 2.0, -2.0, var);
    _vars.push_back(smt_var);
  }

  for (size_t i = 1; i <= var_num; ++i) {
    for (size_t j = i+1; j <= var_num; ++j) {
      std::string var = "b" + std::to_string(i) + std::to_string(j);
      SmtVar* smt_var = new SmtVar("Real", 1.0, -1.0, var);
      _vars.push_back(smt_var);
    }
  }

}

void SmtWriter::initBinVars(unsigned var_num) {
  for (size_t i = 1; i <= var_num; ++i) {
    std::string var = "c" + std::to_string(i);
    SmtVar* smt_var = new SmtVar("Int", 1, -1, var);
    _bin_vars.push_back(smt_var);
  }

  {
    SmtVarUn* smt_var = new SmtVarUn("Real", "g");
    _vars.push_back(smt_var);
  }

  {
    SmtVarUn* smt_var = new SmtVarUn("Real", "k");
    _vars.push_back(smt_var);
  }

}

void SmtWriter::initAsserts2() {

  //1) all valid 
  {
    std::stringstream ss;
    ss << "(assert (=";
    TruthTable::covertable_const_iter c_iter = _truth_table->begin();
    for (; c_iter != _truth_table->end(); ++c_iter) {
      ss << " " <<  _funcs[0]->getFuncExpr(*c_iter, 
          _funcs[1]->getFuncExpr(*c_iter)) ;
    }
    ss << ") )" << std::endl;
    _asserts.push_back(ss.str());
  }

  {
    std::stringstream ss;
    ss << "(assert (= g ";
    TruthTable::covertable_const_iter c_iter = _truth_table->begin();
    ss <<  _funcs[0]->getFuncExpr(*c_iter, _funcs[1]->getFuncExpr(*c_iter));
    ss << ") )" << std::endl;
    _asserts.push_back(ss.str());
  }



  //2) partial valid
  {
    BinIterator bin_iter(4);
    for (; !bin_iter.end(); ++bin_iter) {
      std::vector<int> vals = bin_iter.value();
      std::vector<int> new_vals;
      new_vals.resize(3);
      std::copy(vals.begin(), vals.end(), new_vals.begin());
      CoverTable cov(new_vals);
      CoverTable cov_old(vals);
      if (!_truth_table->isValid(cov.toString())) continue;

      std::stringstream ss;
      ss << "(assert (<=";
      ss << " " << _funcs[0]->getFuncExpr(*(_truth_table->begin()), 
                   _funcs[1]->getFuncExpr(*(_truth_table->begin())));
      ss << " " << _funcs[0]->getFuncExpr(cov_old);
      ss << ") )" << std::endl;
      _asserts.push_back(ss.str());

    }
  }

  //3) non valid
  {
    BinIterator bin_iter(4);
    for (; !bin_iter.end(); ++bin_iter) {
      std::vector<int> vals = bin_iter.value();
      std::vector<int> new_vals;
      new_vals.resize(3);
      std::copy(vals.begin(), vals.end(), new_vals.begin());
      CoverTable cov(new_vals);
      CoverTable cov_old(vals);
      if (_truth_table->isValid(cov.toString())) continue;

      std::stringstream ss;
      ss << "(assert (<";
      ss << " k"; 
      ss << " " << _funcs[0]->getFuncExpr(cov_old);
      ss << ") )" << std::endl;
      _asserts.push_back(ss.str());

    }
  }

  //4)
  {
    std::stringstream ss;
    ss << "(assert (< 3.99 (- k g)))" << std::endl;;
      _asserts.push_back(ss.str());
  }

}

void SmtWriter::initAsserts() {

  //1) all valid 
  {
    std::stringstream ss;
    ss << "(assert (=";
    TruthTable::covertable_const_iter c_iter = _truth_table->begin();
    for (; c_iter != _truth_table->end(); ++c_iter) {
      ss << " " <<  _funcs[0]->getFuncExpr(*c_iter);
    }
    ss << ") )" << std::endl;
    _asserts.push_back(ss.str());
  }


  //2) non-valid cover tables
  BinIterator bin_iter(4);
  for (; !bin_iter.end(); ++bin_iter) {
    CoverTable cov(bin_iter.value());

    if (_truth_table->isValid(cov.toString()))
      continue;


    std::stringstream ss;
    ss << "(assert (<";
    ss << " " << _funcs[0]->getFuncExpr(*(_truth_table->begin()));
    ss << " " << _funcs[0]->getFuncExpr(cov);
    ss << ") )" << std::endl;
    _asserts.push_back(ss.str());
  }



}

void SmtWriter::writeSmt(std::string filename) {
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

  for (size_t i = 0; i < _funcs.size(); ++i)
    outfile << _funcs[i]->getFuncDecl() << std::endl;
  outfile << std::endl;

  std::vector<std::string>::iterator assert_iter = _asserts.begin();
  for (; assert_iter != _asserts.end(); ++assert_iter) {
    outfile << *assert_iter ;
  }
  outfile << std::endl;

  //outfile << "(check-sat)" << std::endl;
  //outfile << "(get-model)" << std::endl;

  outfile.close();
}

SmtWriterBase::~SmtWriterBase() {
  for (size_t i = 0; i < _vars.size(); ++i)
    delete _vars[i];

  for (size_t i = 0; i < _funcs.size(); ++i)
    delete _funcs[i];

  //delete _truth_table;
}






