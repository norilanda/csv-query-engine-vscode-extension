#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../backend/csv_config.h"
#include "../backend/tokenizer.h"
#include "../backend/parser.h"
#include "../backend/binder.h"
#include "../backend/query_executor.h"
#include "../backend/query_exception.h"

std::string executeQuery(const std::string& query, const std::string& csvData) {
    std::stringstream inputStream(csvData);
    std::stringstream outputStream;
    
    std::string header;
    std::getline(inputStream, header, '\n');

    CsvConfig config;
    
    Tokenizer tokenizer(query);
    auto tokens = tokenizer.retrieve_tokens();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    
    Binder binder(ast, config, header);
    binder.bind_column_names_to_column_number();

	Selector selector(ast, config, outputStream);

	ExternalSorter externalSorter(ast.orderByItems, config.fieldDelimeter, selector);
    
    QueryExecutor executor(ast, config, inputStream, header, std::move(externalSorter), selector);
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

// SELECT TESTS

TEST(QueryEngineEndToEnd, SelectAllShouldWork) {
    std::string query = "SELECT *";
    std::string result = executeQuery(query, DEFAULT_CSV);
    
    EXPECT_EQ(result, DEFAULT_CSV);
}

TEST(QueryEngineEndToEnd, SelectSpecificColumnsShouldWork) {
    std::string query = "SELECT city, temperature";
    std::string expected = 
        "city,temperature\n"
        "Prague,20.5\n"
        "London,15.2\n"
        "Brno,18.0\n"
        "Oslo,-5.0\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

// WHERE TESTS

TEST(QueryEngineEndToEnd, WhereClauseNumericShouldWork) {
    std::string query = "SELECT city WHERE temperature > 16.0";
    std::string expected = 
        "city\n"
        "Prague\n"
        "Brno\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, WhereNegativeNumericShouldWork) {
    std::string query = "SELECT city WHERE temperature = -5.0";
    std::string expected = 
        "city\n"
        "Oslo\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, WhereEqualStringShouldWork) {
    std::string query = "SELECT city WHERE weather = 'sun'";
    std::string expected = 
        "city\n"
        "Prague\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, WhereNotEqualStringShouldWork) {
    std::string query = "SELECT city WHERE weather != 'sun'";
    std::string expected = 
        "city\n"
        "London\n"
        "Brno\n"
        "Oslo\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, WhereLessStringShouldWork) {
    std::string query = "SELECT city WHERE city < 'City'";
    std::string expected = 
        "city\n"
        "Brno\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

// AND/OR TESTS

TEST(QueryEngineEndToEnd, WhereOrEqualShouldWork) {
    std::string query = "SELECT city WHERE city = 'Brno' OR city = 'Prague'";
    std::string expected = 
        "city\n"
        "Prague\n"
        "Brno\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, WhereAndShouldWork) {
    std::string query = "SELECT city WHERE is_capital = TRUE AND temperature > 0";
    std::string expected = 
        "city\n"
        "Prague\n"
        "London\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, WhereAndOrShouldHaveCorrectOrder) {
    std::string query = "SELECT city WHERE is_capital = TRUE AND temperature > 0 OR is_capital = FALSE";
    std::string expected = 
        "city\n"
        "Prague\n"
        "London\n"
		"Brno\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, WhereOrAndShouldHaveCorrectOrder) {
    std::string query = "SELECT city WHERE is_capital = FALSE OR is_capital = TRUE AND temperature > 0";
    std::string expected = 
        "city\n"
        "Prague\n"
        "London\n"
		"Brno\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, WhereMutuallyExclusiveShouldReturnNoRecords) {
    std::string query = "SELECT city WHERE is_capital = FALSE AND is_capital = TRUE";
    std::string expected = "city\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, CombinedWhereAndLimitShouldWork) {
    std::string query = "SELECT city WHERE is_capital = TRUE LIMIT 2";
    std::string expected = 
        "city\n"
        "Prague\n"
        "London\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, OrderByShouldWork) {
    std::string query = "SELECT city ORDER BY city";
    std::string expected = 
        "city\n"
		"Brno\n"
        "London\n"
		"Oslo\n"
        "Prague\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, OrderByAscShouldWork) {
    std::string query = "SELECT city ORDER BY city ASC";
    std::string expected = 
        "city\n"
		"Brno\n"
        "London\n"
		"Oslo\n"
        "Prague\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, OrderByDecscShouldWork) {
    std::string query = "SELECT city ORDER BY city DESC";
    std::string expected = 
        "city\n"
		"Prague\n"
        "Oslo\n"
        "London\n"
		"Brno\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, OrderByAscDescShouldWork) {
    std::string query = "SELECT city ORDER BY city ASC, is_capital DESC";
    std::string expected = 
        "city\n"
		"Brno\n"
		"London\n"
        "Oslo\n"
		"Prague\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEndToEnd, OrderByDescAscShouldWork) {
    std::string query = "SELECT city ORDER BY city DESC, is_capital ASC";
    std::string expected = 
        "city\n"
		"Prague\n"
        "Oslo\n"
        "London\n"
		"Brno\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

// EDGE CASE TESTS

TEST(QueryEngineEdgeCases, FakeColumnInSelectShouldThrow) {
    std::string query = "SELECT fake_column";
    
    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), BinderException);
}

TEST(QueryEngineEdgeCases, FakeColumnInWhereShouldThrow) {
    std::string query = "SELECT city WHERE fake_column = 1";
    
    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), BinderException);
}

TEST(QueryEngineEdgeCases, MissingSelectShouldThrow) {
    std::string query = "WHERE temperature > 10";

    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), ParserException);
}

TEST(QueryEngineEdgeCases, MissingWhereShouldThrow) {
    std::string query = "SELECT city temperature > 10";

    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), ParserException);
}

TEST(QueryEngineEdgeCases, MissingCommaShouldThrow) {
    std::string query = "SELECT city temperature";

    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), ParserException);
}

TEST(QueryEngineEdgeCases, InvalidNumericMinusInCenterShouldThrow) {
    std::string query = "SELECT city WHERE temperature = 5-7";

    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), TokenizerException);
}

TEST(QueryEngineEdgeCases, InvalidNumericTwoMinusesShouldThrow) {
    std::string query = "SELECT city WHERE temperature > --57";

    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), TokenizerException);
}

TEST(QueryEngineEdgeCases, InvalidNumericMinusAtEndShouldThrow) {
    std::string query = "SELECT city WHERE temperature > 57-";

    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), TokenizerException);
}

TEST(QueryEngineEdgeCases, StringInsteadOfNumberInWhereShouldWork) { // compare them as strings
    std::string query = "SELECT city WHERE temperature > 'sun'";
    std::string expected = 
        "city\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEdgeCases, NumberInsteadOfStringInWhereShouldWork) { // compare them as strings
    std::string query = "SELECT * WHERE city > -5";

    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), DEFAULT_CSV);
}

TEST(QueryEngineEdgeCases, NumberInsteadOfBoolInWhereShouldWork) { // compare them as strings
    std::string query = "SELECT city WHERE is_capital = 5";
    std::string expected = 
        "city\n";

    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEdgeCases, EmptyQueryShouldThrow) {
    std::string query = "";

    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), QueryException);
}

// LIMIT TESTS

TEST(QueryEngineEdgeCases, LimitExceedsRowCountShouldWork) {
    std::string query = "SELECT city LIMIT 100"; // 100 > actual rows
    std::string expected = 
        "city\n"
        "Prague\n"
        "London\n"
        "Brno\n"
        "Oslo\n";
        
    EXPECT_EQ(executeQuery(query, DEFAULT_CSV), expected);
}

TEST(QueryEngineEdgeCases, LimitNegativeRowCountShouldThrow) {
    std::string query = "SELECT city LIMIT -1";

    EXPECT_THROW(executeQuery(query, DEFAULT_CSV), ParserException);
}