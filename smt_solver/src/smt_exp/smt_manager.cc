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
#include "hw_tgt/tgt_writer.hh"

#include <algorithm>
#include <set>
#include <cassert>
#include <map>
#include <cmath>
#include <fstream>
#include <cstdlib>
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

SmtManager::SmtManager(std::string smt_file, std::vector<int> truth_table, TGT_Graph* graph) :
  _smt_file(smt_file),
  _truth_table(truth_table),
  _tgt_graph(graph),
  _gap(-9999999999999.0) {
    init();
}

void SmtManager::init() {
  TGT_Graph::node_iter n_iter = _tgt_graph->node_begin();
  for (; n_iter != _tgt_graph->node_end(); ++n_iter) {
    TGT_Node* node = n_iter->second;
    _name_to_vars.insert(std::make_pair(node->getName(), 0.0));
  }

  TGT_Graph::edge_iter e_iter = _tgt_graph->edge_begin();
  for (; e_iter != _tgt_graph->edge_end(); ++e_iter) {
    TGT_Edge* edge = *e_iter;
    _name_to_vars.insert(std::make_pair(edge->getName(), 0.0));
  }
}



bool SmtManager::AnalyzeEnergy(double gap) {

  std::string cmd = "cp " + _smt_file + " temp.smt2";
  system(cmd.c_str());

  std::ofstream temp_file;
  temp_file.open("temp.smt2", std::ios::app);
  temp_file << "(assert (< " << gap << " (- k g) ) )" << std::endl;

  z3::context ctx;
  z3::set_param("pp.decimal", true);
  z3::set_param("pp.decimal-precision", 2);

  Z3_ast a = Z3_parse_smtlib2_file(ctx, "temp.smt2", 0, 0, 0, 0, 0, 0);
  z3::expr e(ctx, a);
  z3::solver z3_solver(ctx);
  z3_solver.add(e);
  

  if (z3_solver.check() == z3::unsat) {
    return false;
  } else if (z3_solver.check() == z3::unknown) {
    assert(0);
  } else if (z3_solver.check() == z3::sat) {
    //z3::model m = z3_solver.get_model();

    //for (unsigned i = 0; i < m.size(); ++i) {

    //  assert(m[i].arity() == 0);

    //  std::stringstream s_val;
    //  std::stringstream s_val_name;

    //  s_val << m[i].name();
    //  s_val_name << m.get_const_interp(m[i]);
    //  //std::cout << s_val_name.str() << " " << s_val.str() << std::endl;
    //  setParam(s_val.str(), s_val_name.str());
    //}
    //std::cout << std::endl;
    //findGap();
    return true;
  } else {
    return false;
  }

}

void SmtManager::getModel(double gap) {

  std::string cmd = "cp " + _smt_file + " temp.smt2";
  system(cmd.c_str());

  std::ofstream temp_file;
  temp_file.open("temp.smt2", std::ios::app);
  temp_file << "(assert (< " << gap << " (- k g) ) )" << std::endl;

  z3::context ctx;
  z3::set_param("pp.decimal", true);
  z3::set_param("pp.decimal-precision", 5);

  Z3_ast a = Z3_parse_smtlib2_file(ctx, "temp.smt2", 0, 0, 0, 0, 0, 0);
  z3::expr e(ctx, a);
  z3::solver z3_solver(ctx);
  z3_solver.add(e);
  

  if (z3_solver.check() == z3::unsat) {
    assert(0);
  } else if (z3_solver.check() == z3::unknown) {
    assert(0);
  } else if (z3_solver.check() == z3::sat) {
    z3::model m = z3_solver.get_model();

    for (unsigned i = 0; i < m.size(); ++i) {

      assert(m[i].arity() == 0);

      std::stringstream s_val;
      std::stringstream s_val_name;

      s_val << m[i].name();
      s_val_name << m.get_const_interp(m[i]);
      //std::cout << s_val_name.str() << " " << s_val.str() << std::endl;
      setParam(s_val.str(), s_val_name.str());
    }
    findGap();
  }
}

void SmtManager::Analyze() {
  double delta = 0.1;

  //1) sat
  if (!AnalyzeEnergy(0)) {
    std::cout << "not sat.." << std::endl;
  }

  double max_val = 4 * _tgt_graph->getNodeSize() + 2 * _tgt_graph->getEdgeSize();
  double min_val = 0;

  double med_val = 0;
  while ((max_val - min_val) > delta) {
    med_val = min_val + ((max_val - min_val) / 2);
    if (AnalyzeEnergy(med_val))
      min_val = med_val;
    else
      max_val = med_val;
  }

  assert(max_val >= min_val);
  getModel(min_val);

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

  if (_name_to_vars.count(var_name)) {
    std::cout << "set " << var_name << " " << toDouble(val) << std::endl;
    _name_to_vars[var_name] = toDouble(val);
  } else {
    std::cout << "Cannot find valid variable name " << var_name << std::endl;
    assert(0);
  }

}

void SmtManager::findGap() {

  BinIterator inputs(_tgt_graph->getNodeSize());
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
  assert(inputs.size() == _tgt_graph->getNodeSize());
  double energy = 0.0;

  std::map<std::string, double> name_to_val;
  size_t i = 0;
  std::map<std::string, double>::iterator var_iter = _name_to_vars.begin();
  for (; var_iter != _name_to_vars.end(); ++var_iter, ++i) {
    if (var_iter->first[0] == 'b') continue;
    if (inputs[i] == 0)
      name_to_val.insert(std::make_pair(var_iter->first, -1));
    else if (inputs[i] == 1)
      name_to_val.insert(std::make_pair(var_iter->first, 1));
    else
      assert(0);
  }

  TGT_Graph::node_iter n_iter = _tgt_graph->node_begin();
  for (; n_iter != _tgt_graph->node_end(); ++n_iter) {
    TGT_Node* node = n_iter->second;
    std::string node_name = node->getName();
    energy += name_to_val[node_name] * _name_to_vars[node_name];
  }


  TGT_Graph::edge_iter e_iter = _tgt_graph->edge_begin();
  for (; e_iter != _tgt_graph->edge_end(); ++e_iter) {
    TGT_Edge* edge = *e_iter;
    std::string f_name = edge->getNode(0)->getName();
    std::string t_name = edge->getNode(1)->getName();
    energy += name_to_val[f_name] * name_to_val[t_name] * _name_to_vars[edge->getName()];
  }
    
  return energy;
}

void SmtManager::writeEnergyLandscape(const std::string& outfile) {
  std::ofstream outf;
  outf.open(outfile.c_str());
  BinIterator inputs(_tgt_graph->getNodeSize());
  std::vector<double> energies;
  std::map<std::string, std::vector<int> > remap_input;
  for (; !inputs.end(); ++inputs) { 
    remap_input.insert(std::make_pair(inputs.str(), inputs.value()));
  }
  std::map<std::string, std::vector<int> >::iterator m_iter = remap_input.begin();
  for (; m_iter != remap_input.end(); ++m_iter) {

    std::vector<int> val = m_iter->second;
    for (unsigned i = 0; i < val.size(); ++i)
      outf << val[i];
    outf << " "; 
    outf << evalEnergy(val) ;
    outf << std::endl;
  }

  outf.close();
}



