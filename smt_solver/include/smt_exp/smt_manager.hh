//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      smt_manager.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#ifndef SMT_MANAGER_HH
#define SMT_MANAGER_HH

#include <string>
#include <vector>
#include <map>


double toDouble(const std::string& val);


class TGT_Graph;
class SmtManager {


public:
  SmtManager(std::string smt_file, std::vector<int> truth_table, TGT_Graph* graph);
  ~SmtManager() {}


  void Analyze();
  void writeEnergyLandscape(const std::string& outfile);

private:

  void init();

  unsigned binToInt(const std::vector<int>& table);

  double evalEnergy(const std::vector<int>& inputs);

  void setParam(const std::string& var_name, const std::string& val);
  void findGap();
  bool AnalyzeEnergy(double gap);
  void getModel(double gap);




  std::string _smt_file;
  std::vector<int> _truth_table;
  TGT_Graph* _tgt_graph;
  std::map<std::string, double> _name_to_vars;


  double _gap;
  

};


#endif


