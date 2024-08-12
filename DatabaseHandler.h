#pragma once
#include <windows.h> 
#include <sqltypes.h>
#include <sqlext.h> 
#include <sql.h> 
#include <fstream> 
#include <iostream> 

bool fetchData(const SQLWCHAR* connStr, const SQLWCHAR* query, const char* outputFilename);
