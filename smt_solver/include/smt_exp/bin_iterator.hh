//============================================================================
// Copyright (c) 2017, All Right Reserved, UCLA EDA LAB
//
// file:      bin_iterator.hh
// author:    Juexiao Su
// purpose:   
// revision history:
//============================================================================

#ifndef BIN_ITERATOR_HH
#define BIN_ITERATOR_HH

#include <string>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

class BinIterator {

public:
  BinIterator(size_t width) : width_(width), is_end_(false) {
    bin_.resize(width_, '0') ;
  }

  BinIterator& operator++() {
    std::vector<int> bin = value() ;
    std::vector<int> one ;
    assert(bin.size() == bin_.size()) ;
    one.resize(bin.size()) ;
    one[0] = 1 ;
    for (size_t i = 0; i < bin.size(); i++) {
      bin[i] += one[i] ;
      if (bin[i] > 1) {
        bin[i] %= 2 ;
        if ((i+1) < bin.size())
          bin[i+1]++ ;
        else
          is_end_ = true ;
      }
    }

    for (int i = (int)bin.size()-1; i >=0; i--) {
      bin_[bin.size()-1-(unsigned long)i] = (char)bin[(unsigned long)i] + '0' ;
    }

    //std::cout << bin_ << std::endl ;
    return *this ;

  }

  bool end() {
    return is_end_ ;
  }

  std::vector<int> value() {
    std::vector<int> result ;
    for (int i = (int)bin_.size()-1; i >=0; i--) {
      result.push_back(bin_[(unsigned long)i] - '0') ;
    }
    return result ;
  }

  std::string str() { 
    std::stringstream ss;
    const std::vector<int>& val = value();
    for (unsigned i = 0; i < val.size(); ++i)
      ss << val[i];
    return ss.str();
  }


private:
  std::string bin_;
  size_t width_;
  bool is_end_;

};




#endif

