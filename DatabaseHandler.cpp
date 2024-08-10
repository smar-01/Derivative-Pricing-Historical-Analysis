#include <windows.h> 
#include <sqlext.h> 
#include <sql.h> 
#include <fstream> 
#include <iostream> 
#include "DatabaseHandler.h"


bool fetchData(const SQLWCHAR* connStr, const SQLWCHAR* query, const char* outputFilename)
{
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN ret;

    // Allocate environment handle 
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
    {
        std::cerr << "Error allocating environment handle" << std::endl;
        return -1;
    }

    // Set the ODBC version environment attribute 
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0) != SQL_SUCCESS)
    {
        std::cerr << "Error setting ODBC version" << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return -1;
    }

    // Allocate connection handle 
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
    {
        std::cerr << "Error allocating connection handle" << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return -1;
    }

    // Connect to the SQL Server 
    ret = SQLDriverConnect(hDbc, NULL, (SQLWCHAR*)connStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        std::cerr << "Error connecting to the database" << std::endl;
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc); SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return -1;
    }

    // Allocate statement handle 
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
    {
        std::cerr << "Error allocating statement handle" << std::endl; SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc); SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return -1;
    }

    // SQL query to execute 
    if (SQLExecDirect(hStmt, (SQLWCHAR*)query, SQL_NTS) != SQL_SUCCESS)
    {
        std::cerr << "Error executing SQL query" << std::endl; SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        SQLDisconnect(hDbc); SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return -1;
    }

    // Open a CSV file to write the output 
    std::ofstream file(outputFilename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file for writing" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return -1;
    }

    SQLCHAR columnData[256];
    SQLLEN columnDataLen;

    // Fetch the data and write it to the CSV file 
    while (SQLFetch(hStmt) == SQL_SUCCESS)
    {
        for (int i = 1; ; ++i)
        {
            ret = SQLGetData(hStmt, i, SQL_C_CHAR, columnData, sizeof(columnData), &columnDataLen);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                file << columnData << ",";
            }
            else {
                break;
            }
        }
        file << "\n";
    }
    file.close();

    // Cleanup 
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return true;
}