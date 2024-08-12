

#include <windows.h> 
#include <sql.h> 
#include <sqlext.h> 
#include <sqltypes.h> 
#include <fstream> 
#include <iostream> 
#include <vector>
#include <sstream>
#include "config.h" 
#include "QueryParams.h"
#include "DatabaseHandler.h"
#include "QueryHandler.h"



int main()
{ // Connection string details 
    //connStr already defined
    //Dates already defined
    std::vector<std::wstring> queries = generateQueries(dates, expiries);

    int i = 1; // naming files, make more robust fix later
    for (const auto& querystr : queries) 
    {
        std::wcout << L"Executing query: " << querystr << std::endl;

        const SQLWCHAR* query = querystr.c_str(); //Messy memory leakage but don't know fix yet. just changes string to sqlwchar* so it can be sent to sql db
        std::string outputFilename = "output_" + std::to_string(i++) + ".csv";

        if (!fetchData(connStr, query, outputFilename.c_str()))
        {
            std::wcerr << L"An error occured while fetching data from the database with query."<< std::endl;
            //return -1; // comment out if you want to continue with errors
        }
        else 
        {
            std::cout << "Data exported to " << outputFilename << " successfully." << std::endl;
        }
    }


    return 0;


}