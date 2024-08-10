

#include <windows.h> 
#include <sqlext.h> 
#include <sqltypes.h> 
#include <sql.h> 
#include <fstream> 
#include <iostream> 
#include "config.h" 
#include "DatabaseHandler.h"



int main()
{ // Connection string details 
    //connStr already defined
    const SQLWCHAR* query = L"SELECT TOP (1000) [SecurityID],[Date],[Symbol],[SymbolFlag],[Strike],[Expiration],[CallPut],[BestBid],[BestOffer],[LastTradeDate],[Volume],[OpenInterest],[SpecialSettlement],[ImpliedVolatility],[Delta],[Gamma],[Vega],[Theta],[OptionID],[AdjustmentFactor],[AMSettlement],[ContractSize],[ExpiryIndicator]FROM [IvyDB-USNew].[dbo].[OPTION_PRICE_2023_02] where SecurityID=108105 and date='2023-02-01'";
    const char* outputFilename = "output.csv";

    if (!fetchData(connStr, query, outputFilename))
    {
        std::cerr << "An error occured while fetching data from the database." << std::endl;
        return -1;
    }
    else {
        std::cout << "Data exported to " << outputFilename << " succccessfully." << std::endl;
        return 0;
    }


}