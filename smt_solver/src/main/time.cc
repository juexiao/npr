//============================================================================
// Copyright (c) 2015, All Right Reserved, UCLA EDA GROUP
//
// file:      main.cc
// author:    Tianheng Tu 
// purpose:   report the build time of binary
// revision history:
//============================================================================

#include <stdio.h>

// When we build this file, we use shell 'date' to replace "MAKE DATE TIME"
// and use shell svn to catch svn revision
// it should be invoked when linking in the makefile

namespace nimbus {


char build_time[] = "MAKE_DATE_TIME";

char* getBuildTime() {
  return build_time;
}

char svn_version[] = "SVN_VERSION";

char* getSvnVersion() {
  return svn_version;
}

}
