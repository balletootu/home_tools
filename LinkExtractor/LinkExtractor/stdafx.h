// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include "targetver.h"

//#include <stdio.h>
//#include <tchar.h>

//#include <time.h>
//#include <fstream>
#include <iostream>
//#include <sstream>
//#include <vector>
//#include <deque>
#include <set>
#include <algorithm>
//#include <string>

using namespace std;

//boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/convenience.hpp>

namespace fs = boost::filesystem;

#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }