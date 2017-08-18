//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      tgt_writer.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#include <vector>
#include <string>
#include <map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class TGT_Node;
class TGT_Edge;
class TGT_Writer;

using boost::property_tree::ptree;

class TGT_Node {

public:
  TGT_Node(std::string name, unsigned index, int assign) : _name(name),
  _index(index),
  _assign(assign) {}

  void addEdge(TGT_Edge* edge) { _edges.push_back(edge); }

  std::string getName() const { return _name; }
  int getAssign() const { return _assign; }
  unsigned getIndex() const { return _index; }

private:
  std::vector<TGT_Edge*> _edges;
  std::string _name;

  unsigned _index;
  int _assign;

};


class TGT_Edge {

public:
  TGT_Edge(TGT_Node* node1, TGT_Node* node2, std::string name) : _name(name) {
    _nodes.push_back(node1);
    _nodes.push_back(node2);
  }

  std::string getName() const { return _name; }
  TGT_Node* getNode(unsigned i) const { return _nodes[i]; }


private:
  std::vector<TGT_Node*> _nodes;
  std::string _name;

};


class TGT_Graph {
public:
  typedef std::map<std::string, TGT_Node*> NameToNode;
  typedef NameToNode::iterator node_iter;
  typedef std::vector<TGT_Edge*>::iterator edge_iter;
public:
  TGT_Graph() {}
  ~TGT_Graph();

  void initChimeraGraph();
  void initChimeraGraph33();
  void buildPtree();
  void writeJson(std::string filename);

  void loadJson(std::string filename);

  void clear();

  node_iter node_begin() { return _nodes.begin(); }
  node_iter node_end() { return _nodes.end(); }

  edge_iter edge_begin() { return _edges.begin(); }
  edge_iter edge_end() { return _edges.end(); }

  size_t getNodeSize() { return _nodes.size(); }
  

private:

  NameToNode _nodes;
  std::vector<TGT_Edge*> _edges;
  ptree _root;

};


