#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "../backend/csv_config.h"
#include "../backend/tokenizer.h"
#include "../backend/parser.h"
#include "../backend/binder.h"
#include "../backend/query_executor.h"
#include "../backend/query_exception.h"

enum City {
	Brno,
	London,
	Oslo,
	Prague
};

std::string executeQueryExternalSort(const std::string& query, const std::string& csvData, size_t memoryLimit) {
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

    ExternalSorter externalSorter(ast.orderByItems, config.fieldDelimeter, selector, memoryLimit);
    
    QueryExecutor executor(ast, config, inputStream, header, std::move(externalSorter), selector);
    executor.run();

    return outputStream.str();
}

std::string generateBigCsv(int multiplier) {
    std::stringstream ss;
    ss << "city,weather,temperature,is_capital\n";
    for (int i = 0; i < multiplier; ++i) {
        // use something dynamic or just duplicate it
        ss << "Prague,sun,20.5,TRUE\n";
        ss << "London,rain,15.2,TRUE\n";
        ss << "Brno,cloudy,18.0,FALSE\n";
        ss << "Oslo,snow,-5.0,TRUE\n";
    }
    return ss.str();
}

std::string generateBigExpectedCsv(
    int multiplier,
    std::map<City, bool> filter = {
        {Brno, true},
        {London, true},
        {Oslo, true},
        {Prague, true}
    })
{
    std::stringstream ss;
    ss << "city\n";

    auto writeMultipleTimes = [&](const std::string& city) {
        for (int i = 0; i < multiplier; ++i) {
            ss << city << "\n";
        }
	};

	if (filter[Brno]) {
        writeMultipleTimes("Brno");
    }
    if (filter[London]) {
        writeMultipleTimes("London");
	}

    if (filter[Oslo]) {
        writeMultipleTimes("Oslo");
    }
    if (filter[Prague]) {
        writeMultipleTimes("Prague");
	}

    return ss.str();
}

TEST(QueryEngineExternalSort, OrderByShouldWorkWithExternalSortLimit200Bytes) {
    std::string query = "SELECT city ORDER BY city ASC";
    
    int multiplier = 50; 
    std::string data = generateBigCsv(multiplier);
    std::string expected = generateBigExpectedCsv(multiplier);
    
    std::string result = executeQueryExternalSort(query, data, 200);
    
    EXPECT_EQ(result, expected);
}

TEST(QueryEngineExternalSort, OrderByShouldWorkWithExternalSortLimit1000Bytes) {
    std::string query = "SELECT city ORDER BY city ASC";
    
    int multiplier = 50; 
    std::string data = generateBigCsv(multiplier);
    std::string expected = generateBigExpectedCsv(multiplier);
    
    std::string result = executeQueryExternalSort(query, data, 1000);
    
    EXPECT_EQ(result, expected);
}

TEST(QueryEngineExternalSort, OrderByWhereShouldWorkWithExternalSortLimit1000Bytes) {
    std::string query = "SELECT city WHERE city = 'Prague' OR city = 'Brno' ORDER BY city ASC";
    
    int multiplier = 50; 
    std::string data = generateBigCsv(multiplier);
    std::string expected = generateBigExpectedCsv(multiplier, {
        {Brno, true},
        {London, false},
        {Oslo, false},
        {Prague, true}
    });
    
    std::string result = executeQueryExternalSort(query, data, 1000);
    
    EXPECT_EQ(result, expected);
}
