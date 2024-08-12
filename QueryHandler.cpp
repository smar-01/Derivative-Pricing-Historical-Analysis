
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "QueryHandler.h"

// Create SQL query modifiable SecurityID and Date
std::wstring createQuery(const std::wstring& date, const std::wstring& expiry) 
{
    std::wstringstream query;
    query << L"SELECT TOP (1000) [SecurityID],[Date],[Symbol],[SymbolFlag],[Strike],[Expiration],[CallPut],[BestBid],[BestOffer],[LastTradeDate],[Volume],[OpenInterest],[SpecialSettlement],[ImpliedVolatility],[Delta],[Gamma],[Vega],[Theta],[OptionID],[AdjustmentFactor],[AMSettlement],[ContractSize],[ExpiryIndicator] FROM [IvyDB-USNew].[dbo].[OPTION_PRICE_2023_01] WHERE SecurityID=108105 and date='" << date << L"' and Expiration='" << expiry << L"'";
    return query.str();
}

// Generate list of queries
std::vector<std::wstring> generateQueries(const std::vector<std::wstring>& dates, const std::vector<std::wstring> expiries)
{
    std::vector<std::wstring> queries;

    if (dates.size() != expiries.size()) {
        std::cerr << "Error: Dates and expiries must have the same number of elements." << std::endl;
        return queries; // Return an empty vector or handle the error as appropriate
    }

    for (size_t i = 0; i < dates.size(); ++i) {
        queries.push_back(createQuery(dates[i], expiries[i]));
    }

    return queries;
}

