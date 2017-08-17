//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      tgt_writer.cc
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#include "hw_tgt/tgt_writer.hh"

#include <fstream>


TGT_Graph::~TGT_Graph() {

  NameToNode::iterator n_iter = _nodes.begin();
  for (; n_iter != _nodes.end(); ++n_iter)
    delete n_iter->second;
  _nodes.clear();


  for (unsigned i = 0; i < _edges.size(); ++i)
    delete _edges[i];

  _edges.clear();

}

void TGT_Graph::clear() {

  NameToNode::iterator n_iter = _nodes.begin();
  for (; n_iter != _nodes.end(); ++n_iter)
    delete n_iter->second;
  _nodes.clear();


  for (unsigned i = 0; i < _edges.size(); ++i)
    delete _edges[i];

  _edges.clear();

}

void TGT_Graph::initChimeraGraph() {

  for (unsigned i = 0; i < 8; ++i) {
    TGT_Node* node = new TGT_Node("a" + std::to_string(i));
    _nodes.insert(std::make_pair(node->getName(), node));
  }

  for (unsigned i = 0; i < 4; ++i) {
    for (unsigned j = 0; j < 4; ++j) {
      TGT_Edge* edge = new TGT_Edge(
          _nodes.at("a"+std::to_string(i)), 
          _nodes.at("a"+std::to_string(4+j)), 
          "b" + std::to_string(i) + std::to_string(4+j));

      _edges.push_back(edge);
    }
  }
}

void TGT_Graph::buildPtree() {
  ptree nodes;
  ptree edges;

  NameToNode::iterator n_iter = _nodes.begin();
  for (; n_iter != _nodes.end(); ++n_iter) {
    ptree node;
    node.put("Name", n_iter->second->getName());
    nodes.push_back(std::make_pair("", node));
  }

  for (size_t i = 0; i < _edges.size(); ++i) {
    ptree edge;
    edge.put("Name", _edges[i]->getName());
    edge.put("Node0", _edges[i]->getNode(0)->getName());
    edge.put("Node1", _edges[i]->getNode(1)->getName());
    edges.push_back(std::make_pair("", edge));
  }

  _root.add_child("Nodes", nodes);
  _root.add_child("Edges", edges);
}

void TGT_Graph::writeJson(std::string filename) {
  std::ofstream outfile;
  outfile.open(filename.c_str());

  boost::property_tree::write_json(outfile, _root);
  outfile.close();
}

void TGT_Graph::loadJson(std::string filename) {
  std::ifstream infile;

  infile.open(filename.c_str());
  _root.clear();

  boost::property_tree::read_json(infile, _root);

  clear();

  {
    const ptree& nodes = _root.get_child("Nodes");
    ptree::const_iterator p_iter = nodes.begin();
    for (; p_iter != nodes.end(); ++p_iter) {
      const ptree& node = p_iter->second;
      TGT_Node* node_p = new TGT_Node(node.get<std::string>("Name"));
      _nodes.insert(std::make_pair(node_p->getName(), node_p));
    }
  }

  {
    const ptree& edges = _root.get_child("Edges");
    ptree::const_iterator p_iter = edges.begin();
    for (; p_iter != edges.end(); ++p_iter) {
      const ptree& edge = p_iter->second;
      std::string node0 = edge.get<std::string>("Node0");
      std::string node1 = edge.get<std::string>("Node1");
      TGT_Edge* edge_p = new TGT_Edge(_nodes[node0], _nodes[node1], edge.get<std::string>("Name"));
      _edges.push_back(edge_p);
    }
  }
  infile.close();
}



