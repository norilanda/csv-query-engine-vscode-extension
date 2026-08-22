#include <gtest/gtest.h>
#include <sstream>
#include <string>

// project headers
#include "../backend/csv_config.h"
#include "../backend/tokenizer.h"
#include "../backend/parser.h"
#include "../backend/binder.h"
#include "../backend/query_executor.h"
#include "../backend/query_exception.h"

// --- Helper Function ---
// This takes a query and CSV string, runs your engine, and returns the output string.
std::string executeQuery(const std::string& query, const std::string& csvData) {
    std::stringstream inputStream(csvData);
    std::stringstream outputStream;
    
    // Extract header for Binder
    std::string header;
    std::getline(inputStream, header, '\n');

    CsvConfig config;
    
    Tokenizer tokenizer(query);
    auto tokens = tokenizer.retrieve_tokens();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    
    Binder binder(ast, config, header);
    binder.bind_column_names_to_column_number();
    
    QueryExecutor executor(ast, config, inputStream, outputStream, header);
    executor.run();

    return outputStream.str();
}

// --- Default Test Data ---
const std::string DEFAULT_CSV = 
    "city,weather,temperature,is_capital\n"
    "Prague,sun,20.5,TRUE\n"
    "London,rain,15.2,TRUE\n"
    "Brno,cloudy,18.0,FALSE\n"
    "Oslo,snow,-5.0,TRUE\n";

// ==========================================
//                 TESTS
// ==========================================

TEST(QueryEngineEndToEnd, SelectAll) {
    std::string query = "SELECT *";
    std::string result = executeQuery(query, DEFAULT_CSV);
    
    EXPECT_EQ(result, DEFAULT_CSV);
}

TEST(QueryEngineEndToEnd, SelectSpecificColumns) {
    std::string query = "SELECT city, temperature";
    std::string expected = 
        "city,temperature\n"
        "Prague,20.5\n"
        "London,15.2\n"
        "Brno,18.0\n"
        "Oslo,-5.0\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, WhereClauseNumeric) {
    std::string query = "SELECT city WHERE temperature > 16.0";
    std::string expected = 
        "city\n"
        "Prague\n"
        "Brno\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, CombinedWhereAndLimit) {
    std::string query = "SELECT city WHERE is_capital = TRUE LIMIT 2";
    std::string expected = 
        "city\n"
        "Prague\n"
        "London\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}
TEST(QueryEngineEdgeCases, InvalidColumnThrowsException) {
    std::string query = "SELECT fake_column";
    
    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), BinderException);
}

TEST(QueryEngineEdgeCases, MissingSelectThrowsException) {
    std::string query = "WHERE temperature > 10";
    
    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), ParserException);
}

TEST(QueryEngineEdgeCases, EmptyQueryThrowsException) {
    std::string query = "";
    
    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), QueryException);
}

TEST(QueryEngineEdgeCases, LimitExceedsRowCount) {
    std::string query = "SELECT city LIMIT 100"; // 100 > actual rows
    std::string expected = 
        "city\n"
        "Prague\n"
        "London\n"
        "Brno\n"
        "Oslo\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}
