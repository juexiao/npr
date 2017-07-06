//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      truth_table_gen.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#ifndef TRUTH_TABLE_GEN_HH
#define TRUTH_TABLE_GEN_HH


#include <string>
#include <sstream>


class TruthTableGenerator {


public:

  TruthTableGenerator(unsigned table_size) :
    _table_size(table_size) {
      _path = "./truth_tables_input_";
      std::stringstream ss;
      ss << table_size;
      _path += ss.str();
      std::string cmd = "mkdir -p " + _path;
      system(cmd.c_str());
    }


  void generateAllTruthTable();


private:

  unsigned _table_size;   //truth table variable number;
  std::string _path;

};

#endif


