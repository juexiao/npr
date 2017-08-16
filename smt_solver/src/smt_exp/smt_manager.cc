//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      smt_solver.cc
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#include "smt_exp/smt_manager.hh"
#include "smt_exp/bin_iterator.hh"

#include <algorithm>
#include <set>
#include <cassert>
#include <fstream>
#include <z3++.h>


bool my_predicate(char c) { return (c==' ' || c=='(' || c==')'); }
double toDouble(const std::string& val) {

  std::string copy_val = val;
  double res;
  copy_val.erase(std::remove_if(copy_val.begin(), copy_val.end(), my_predicate), copy_val.end());
  std::stringstream ss;
  ss << copy_val;
  ss >> res;
  return res;

}



std::string SmtManager::AnalyzeEnergy() {
 
  std::stringstream ss;
  z3::context ctx;
  z3::set_param("pp.decimal", true);
  z3::set_param("pp.decimal-precision", 5);

  Z3_ast a = Z3_parse_smtlib2_file(ctx, _smt_file.c_str(), 0, 0, 0, 0, 0, 0);
  z3::expr e(ctx, a);
  z3::solver z3_solver(ctx);
  z3_solver.add(e);
  
  ss << binToInt(_truth_table) << ",";

  if (z3_solver.check() == z3::unsat) {
    ss << "unsat,0";
  } else if (z3_solver.check() == z3::unknown) {
    ss << "unknown,0";
  } else if (z3_solver.check() == z3::sat) {
    z3::model m = z3_solver.get_model();

    for (unsigned i = 0; i < m.size(); ++i) {

      assert(m[i].arity() == 0);

      std::stringstream s_val;
      std::stringstream s_val_name;

      s_val << m[i].name();
      s_val_name << m.get_const_interp(m[i]);
      std::cout << s_val_name.str() << " " << s_val.str() << std::endl;
      setParam(s_val.str(), s_val_name.str());
    }
    std::cout << std::endl;
    findGap();
    ss << "sat," << _gap;
  }

  return ss.str();
}

unsigned SmtManager::binToInt(const std::vector<int>& table) {

  std::vector<int>::const_reverse_iterator t_iter = table.rbegin();

  unsigned result = 0;
  unsigned index = 0;
  for (; t_iter != table.rend(); ++t_iter, ++index) {
    int val = *t_iter;
    assert(val == 0 || val == 1);
    result += (unsigned)(val << index);

  }

  return result;
}


void SmtManager::setParam(const std::string& var_name, const std::string& val) {

  if (var_name[0] == 'c') return;
  if (var_name[0] == 'k') return;
  if (var_name[0] == 'g') return;

  if (var_name == "a1") {
    a1 = toDouble(val);
  } else if (var_name == "a2") {
    a2 = toDouble(val);
  } else if (var_name == "a3") {
    a3 = toDouble(val);
  } else if (var_name == "a4") {
    a4 = toDouble(val);
  } else if (var_name == "b12") {
    b12 = toDouble(val);
  } else if (var_name == "b13") {
    b13 = toDouble(val);
  } else if (var_name == "b14") {
    b14 = toDouble(val);
  } else if (var_name == "b23") {
    b23 = toDouble(val);
  } else if (var_name == "b24") {
    b24 = toDouble(val);
  } else if (var_name == "b34") {
    b34 = toDouble(val);
  } else {
    std::cout << "Cannot find valid variable name " << var_name << std::endl;
    assert(0);
  }

}

void SmtManager::findGap() {
  assert(a1 >= -2 && a1 <= 2);
  assert(a2 >= -2 && a2 <= 2);
  assert(a3 >= -2 && a3 <= 2);
  assert(a4 >= -2 && a4 <= 2);
  assert(b12 >= -1 && b12 <= 1);
  assert(b13 >= -1 && b13 <= 1);
  assert(b14 >= -1 && b14 <= 1);
  assert(b23 >= -1 && b23 <= 1);
  assert(b24 >= -1 && b24 <= 1);
  assert(b34 >= -1 && b34 <= 1);

  BinIterator inputs(4);
  std::vector<double> energies;
  for (; !inputs.end(); ++inputs) { 
    energies.push_back(evalEnergy(inputs.value())) ;
  }

  std::set<double> unique_energy;
  for (size_t i = 0; i < energies.size(); ++i)
    unique_energy.insert(energies[i]);

  assert(unique_energy.size() >=2 );

  _gap = *(++unique_energy.begin()) - *(unique_energy.begin());
}

double SmtManager::evalEnergy(const std::vector<int>& inputs) {
  assert(inputs.size() == 4);
  double energy = 0.0;

  std::vector<double> vals;
  for (size_t i = 0; i < inputs.size(); ++i) {
    if (inputs[i] == 0)
      vals.push_back(-1.0);
    else if (inputs[i] == 1)
      vals.push_back(1.0);
    else
      assert(0);
  }
  assert(vals.size() == 4);

  energy = (vals[0] * a1) + (vals[1] * a2) + (vals[2] * a3) + (vals[3] * a4) +
           (vals[0] * vals[1] * b12) + (vals[0] * vals[2] * b13) + (vals[0] * vals[3] * b14) +
           (vals[1] * vals[2] * b23) + (vals[1] * vals[3] * b24) +
           (vals[2] * vals[3] * b34);


  return energy;


}

void SmtManager::writeEnergyLandscape(const std::string& outfile) {
  std::ofstream outf;
  outf.open(outfile.c_str());
  BinIterator inputs(4);
  std::vector<double> energies;
  for (; !inputs.end(); ++inputs) { 
    for (unsigned i = 0; i < inputs.value().size(); ++i)
      outf << inputs.value()[i];
    outf << " "; 
    outf << evalEnergy(inputs.value()) ;
    outf << std::endl;
  }
  outf.close();


}



