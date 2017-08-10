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


double toDouble(const std::string& val);


class SmtManager {


public:
  SmtManager(std::string smt_file, std::vector<int> truth_table) :
  _smt_file(smt_file),
  _truth_table(truth_table),
  a1(-3),
  a2(-3),
  a3(-3),
  a4(-3),
  b12(-3),
  b13(-3),
  b14(-3),
  b23(-3),
  b24(-3),
  b34(-3),
  _gap(-999)
  {}


  std::string AnalyzeEnergy();
  void writeEnergyLandscape(const std::string& outfile);

private:

  unsigned binToInt(const std::vector<int>& table);

  double evalEnergy(const std::vector<int>& inputs);

  void setParam(const std::string& var_name, const std::string& val);
  void findGap();




  std::string _smt_file;
  std::vector<int> _truth_table;

  double a1;
  double a2;
  double a3;
  double a4;
  double b12;
  double b13;
  double b14;
  double b23;
  double b24;
  double b34;

  double _gap;
  

};


#endif


