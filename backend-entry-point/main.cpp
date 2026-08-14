#include <iostream>
#include <fstream>
#include <string>

#include "../backend/file_helper.h"
#include "../backend/csv_config.h"
#include "../backend/tokenizer.h"
#include "../backend/parser.h"
#include "../backend/binder.h"
#include "../backend/query_executor.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No input provided" << std::endl;
        return 1;
    }
    
    std::string input = argv[1];
    
    // CSV processing logic would go here
    // For now, simple string manipulation
    std::string output = "processed " + input;

    // Output the result (extension will read from stdout)
    std::cout << output << std::endl;

    // ---------------------------

    std::string queryInput = "SELECT weather, temperature WHERE weather = 'sun'";
    std::string queryInput1 = "SELECT * LIMIT 1";

    std::string header = "city,weather,temperature";
	std::string directoryPath = "D:\\Old-Projects\\Charles-University\\csv-query-extension\\backend-entry-point\\";

	std::ifstream inFile;
	FileHelper::open_file_to_read(directoryPath, "input.csv", inFile);

	std::ofstream outFile;
	FileHelper::create_and_open_file_to_write(directoryPath, "output.csv", outFile);

    CsvConfig config;

    Tokenizer tokenier(queryInput);
    auto tokens = tokenier.retrieve_tokens();
    Parser parser(tokens);
    QueryAST ast = parser.parse();
    Binder binder(ast, config, header);
    binder.bind_column_names_to_column_number();
    QueryExecutor executor(ast, config, inFile, outFile);
	executor.run();

	// ---------------------------

    Tokenizer tokenier1(queryInput1);
    auto tokens1 = tokenier1.retrieve_tokens();
    Parser parser1(tokens1);
    auto ast1 = parser1.parse();
    Binder binder1(ast1, config, header);
    binder1.bind_column_names_to_column_number();

    
    return 0;
}