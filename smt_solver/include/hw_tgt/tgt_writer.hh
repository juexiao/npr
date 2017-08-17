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
  TGT_Node(std::string name) : _name(name) {}

  void addEdge(TGT_Edge* edge) { _edges.push_back(edge); }

  std::string getName() const { return _name; }

private:
  std::vector<TGT_Edge*> _edges;
  std::string _name;

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
  typedef std::map<std::string, TGT_Node*> NameToNode;
public:
  TGT_Graph() {}
  ~TGT_Graph();

  void initChimeraGraph();
  void buildPtree();
  void writeJson(std::string filename);

  void loadJson(std::string filename);

  void clear();
private:

  NameToNode _nodes;
  std::vector<TGT_Edge*> _edges;
  ptree _root;

};


