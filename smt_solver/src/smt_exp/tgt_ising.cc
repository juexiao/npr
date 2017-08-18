//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      tgt_ising.cc
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#include "smt_exp/tgt_ising.hh"
#include "smt_exp/assist3.hh"

#include <set>


//TGT_Ising::TGT_Ising(std::string topofile, std::string name) :
//SmtFunction(name) {
//  _tgt_graph = new TGT_Graph();
//  _tgt_graph->loadJson(topofile);
//}

TGT_Ising::TGT_Ising(TGT_Graph* graph, std::string name) :
  _tgt_graph(graph),
  SmtFunction(name) {}

TGT_Ising::~TGT_Ising() {
  //if (_tgt_graph)
  //  delete _tgt_graph;

  for (size_t i = 0; i < _expressions.size(); ++i) {
    delete _expressions[i];
  }
  _expressions.clear();

  for (size_t i =0; i < _inputs.size(); ++i) {
    delete _inputs[i];
  }
  _inputs.clear();

  std::map<unsigned, AssistX*>::iterator ass_iter = _index_to_func.begin();
  for (; ass_iter != _index_to_func.end(); ++ass_iter)
    delete ass_iter->second;

  _index_to_func.clear();

}


void TGT_Ising::init() {
  _return_type = "Real";
  
  initVars();
  initInputs();
  initAssitFunc();
  initExprs();


}

void TGT_Ising::graphCheck() {
  std::set<int> assigns;
  for (size_t i = 0; i < _input_assign.size(); ++i) {
    if (_input_assign[i] != -1)
      assigns.insert(_input_assign[i]);
  }

  for (int i = 0; i < (int)assigns.size(); ++i) {
    assert(assigns.count(i));
  }
}

void TGT_Ising::initInputs() {
  size_t node_num = _tgt_graph->getNodeSize();
  _inputs.resize(node_num);
  _input_assign.resize(node_num);

  TGT_Graph::node_iter n_iter = _tgt_graph->node_begin();
  TGT_Graph::node_iter n_end = _tgt_graph->node_end();

  for (; n_iter != n_end; ++n_iter) {
    TGT_Node* t_node = n_iter->second;
    unsigned n_index = t_node->getIndex();
    int n_assign = t_node->getAssign();
    _inputs[n_index] = new SmtInput("q" + std::to_string(n_index), "Real");
    _input_assign[n_index] = n_assign;
  }

  graphCheck();

};

void TGT_Ising::initVars() {
  TGT_Graph::node_iter n_iter = _tgt_graph->node_begin();
  TGT_Graph::node_iter n_end = _tgt_graph->node_end();
  for (; n_iter != n_end; ++n_iter) {
    SmtVar* var = new SmtVar("Real", 2.0, -2.0, n_iter->first);
    _name_to_var.insert(std::make_pair(n_iter->first, var));
  }

  TGT_Graph::edge_iter e_iter = _tgt_graph->edge_begin();
  TGT_Graph::edge_iter e_end = _tgt_graph->edge_end();

  for (; e_iter != e_end; ++e_iter) {
    SmtVar* var = new SmtVar("Real", 1.0, -1.0, (*e_iter)->getName());
    _name_to_var.insert(std::make_pair((*e_iter)->getName(), var));
  }
};

void TGT_Ising::initExprs() {
  TGT_Graph::node_iter n_iter = _tgt_graph->node_begin();
  TGT_Graph::node_iter n_end = _tgt_graph->node_end();
  for (; n_iter != n_end; ++n_iter) {
    unsigned index = n_iter->second->getIndex();
    std::string node_name = n_iter->second->getName();
    SmtInput* inpt = _inputs[index];
    SmtExpression* expr = new SmtExpression("*", inpt->getName(), node_name, "");
    _expressions.push_back(expr);
  }

  TGT_Graph::edge_iter e_iter = _tgt_graph->edge_begin();
  TGT_Graph::edge_iter e_end = _tgt_graph->edge_end();
  for (; e_iter != e_end; ++e_iter) {
    unsigned index0 = (*e_iter)->getNode(0)->getIndex();
    unsigned index1 = (*e_iter)->getNode(1)->getIndex();
    SmtInput* inpt0 = _inputs[index0];
    SmtInput* inpt1 = _inputs[index1];
    std::string edge_name = (*e_iter)->getName();
    SmtExpression* expr = new SmtExpression("*", 
        inpt0->getName(), inpt1->getName(), edge_name);
    _expressions.push_back(expr);
  }
}

void TGT_Ising::initAssitFunc() {
  unsigned input_num = getNonAssistNum();
  unsigned ass_count = 0;
  unsigned total_count = 0;
  for (size_t i = 0; i < _input_assign.size(); ++i) {
    if (_input_assign[i] == -1) {
      AssistX* new_function = new AssistX("assist" + std::to_string(i),
          input_num, ass_count * (1 << input_num));
      new_function->init();
      _index_to_func.insert(std::make_pair(i, new_function));
      ++ass_count;
      total_count += (1 << input_num);
    }
  }

  for (unsigned i = 0; i < total_count; ++i) {
    std::string var = "c" + std::to_string(i);
    SmtVar* smt_var = new SmtVar("Int", 1, -1, var);
    _name_to_bin_var.insert(std::make_pair(var, smt_var));
  }
}

unsigned TGT_Ising::getNonAssistNum() const {
  unsigned counta = 0;
  for (size_t i = 0; i < _input_assign.size(); ++i) {
    if (_input_assign[i] != -1)
      ++counta;
  }
  return counta;
}

unsigned TGT_Ising::getAssistNum() const{
  unsigned counta = 0;
  for (size_t i = 0; i < _input_assign.size(); ++i) {
    if (_input_assign[i] == -1)
      ++counta;
  }
  return counta;
}

std::string TGT_Ising::getFuncDecl() const {
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
  for (size_t i = 0; i < _expressions.size(); ++i)
    ss << " " << _expressions[i]->getExpr();
  ss << ")" << std::endl;

  //3) tail
  ss << ")" << std::endl;
  return ss.str();

}

std::string TGT_Ising::getFuncExpr(const CoverTable& cov) const {
  assert(cov.size() == _input_assign.size());
  std::stringstream ss;
  ss << "(" << _name;
  for (size_t i = 0; i < cov.size(); ++i) {
    assert(cov[i] == 1 || cov[i] == -1);
    ss << " " << cov[i];
  }
  ss << ")";

  return ss.str();
}

std::string TGT_Ising::getFuncExprAssist(const CoverTable& cov) const {
  assert(cov.size() == getNonAssistNum());
  std::stringstream ss;
  ss << "(" << _name;
  for (unsigned int i = 0; i < _input_assign.size(); ++i) {
    if (_input_assign[i] == -1) {
      ss << " " << _index_to_func.at(i)->getFuncExpr(cov);
    } else {
      ss << " " << cov[(unsigned)_input_assign[i]];
    }
  }
  ss << ")";
  return ss.str();

}

std::vector<SmtVar*> TGT_Ising::getVarVec() {
  std::vector<SmtVar*> vars;
  std::map<std::string, SmtVar*>::iterator s_iter = _name_to_var.begin();
  for (; s_iter != _name_to_var.end(); ++s_iter) {
    vars.push_back(s_iter->second);
  }
  return vars;
}

std::vector<SmtVar*> TGT_Ising::getBinVarVec() {
  std::vector<SmtVar*> vars;
  std::map<std::string, SmtVar*>::iterator s_iter = _name_to_bin_var.begin();
  for (; s_iter != _name_to_bin_var.end(); ++s_iter) {
    vars.push_back(s_iter->second);
  }
  return vars;
}

std::vector<AssistX*> TGT_Ising::getAssistFunctions() {
  std::vector<AssistX*> funcs;
  std::map<unsigned, AssistX*>::iterator func_iter = _index_to_func.begin();
  for (; func_iter != _index_to_func.end(); ++func_iter) {
    funcs.push_back(func_iter->second);
  }
  return funcs;

}



