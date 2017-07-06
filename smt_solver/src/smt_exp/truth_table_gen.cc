//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      truth_table_gen.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================



#include "smt_exp/truth_table_gen.hh"
#include "smt_exp/bin_iterator.hh"

#include <cmath>
#include <fstream>
#include <vector>
#include <cstdlib>


std::string toString(unsigned int i) {

  std::stringstream ss;
  ss << i;
  return ss.str();
}

void TruthTableGenerator::generateAllTruthTable() {

  unsigned entry_num = 1 << _table_size;

  BinIterator truth_iter(entry_num);
  unsigned int index = 0;

  for (; !truth_iter.end(); ++truth_iter, ++index) {
    std::string outfilename = _path + "/truth_table_" +  toString(index);
    std::ofstream outfile;
    std::vector<int> truth_table = truth_iter.value();

    outfile.open(outfilename.c_str());
    BinIterator input_var(_table_size);

    unsigned int entry_index = 0;
    for(; !input_var.end(); ++input_var, ++entry_index) {
      std::vector<int> vals = input_var.value();
      for (unsigned int i = 0; i < vals.size(); ++i)
        outfile << vals[i];
      outfile << truth_table[entry_index] << std::endl;

    }
    outfile.close();
  }

}

