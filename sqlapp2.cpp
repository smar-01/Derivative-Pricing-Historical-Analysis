
#include <windows.h> 
#include <sqlext.h> 
#include <sqltypes.h> 
#include <sql.h> 
#include <fstream> 
#include <iostream> 
#include "db_config.h"

bool databaseConnect(SQLHENV& hEnv, SQLHDBC& hDbc, const SQLWCHAR* connStr)
{
    SQLRETURN ret;

    // Allocate environment handle 
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
    {
        std::cerr << "Error allocating environment handle" << std::endl;
        return false;
    }

    // Set the ODBC version environment attribute 
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0) != SQL_SUCCESS)
    {
        std::cerr << "Error setting ODBC version" << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return false;
    }

    // Allocate connection handle 
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
    {
        std::cerr << "Error allocating connection handle" << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return false;
    }

    // Connect to the SQL Server 
    ret = SQLDriverConnect(hDbc, NULL, (SQLWCHAR*)connStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        std::cerr << "Error connecting to the database" << std::endl;
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc); SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return false;
    }

    std::cout << "Connected to SQL Server successfully." << std::endl;
    return true;

}

bool executeQueryExportCSV(SQLHDBC hDbc, const SQLWCHAR* query, const char* outputFilename)
{
    SQLHSTMT hStmt;
    SQLRETURN ret;
    
    // Allocate statement handle 
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
    {
        std::cerr << "Error allocating statement handle" << std::endl; SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc); SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return false;
    }

    // SQL query to execute 
    if (SQLExecDirect(hStmt, (SQLWCHAR*)query, SQL_NTS) != SQL_SUCCESS)
    {
        std::cerr << "Error executing SQL query" << std::endl; 
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }

    std::cout << "SQL query executed successfully." << std::endl;

    // Open a CSV file to write the output 
    std::ofstream file(outputFilename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file for writing" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }

    SQLCHAR columnData[256];
    SQLLEN columnDataLen;

    // Fetch the data & write it the CSV file 
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
}

bool fetchData(const SQLWCHAR* connStr, const SQLWCHAR* query, const char* outputFilename)
{
    SQLHENV hEnv;
    SQLHDBC hDbc;

 
    

    

    // Cleanup 
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return true;
}

int main() 
{ // Connection string details
    //connStr defined in config.h
    const SQLWCHAR* query = L"SELECT TOP (1000) [SecurityID],[Date],[Symbol],[SymbolFlag],[Strike],[Expiration],[CallPut],[BestBid],[BestOffer],[LastTradeDate],[Volume],[OpenInterest],[SpecialSettlement],[ImpliedVolatility],[Delta],[Gamma],[Vega],[Theta],[OptionID],[AdjustmentFactor],[AMSettlement],[ContractSize],[ExpiryIndicator]FROM [IvyDB-USNew].[dbo].[OPTION_PRICE_2023_02] where SecurityID=108105 and date='2023-02-01'";
    const char* outputFilename = "output.csv";

    if (!fetchData(connStr, query, outputFilename))
    {
        std::cerr << "An error occured while fetching data from the database." << std::endl;
        return -1;
    } else {
        std::cout << "Data exported to " << outputFilename << " successfully." << std::endl;
        return 0;
    }

    
}