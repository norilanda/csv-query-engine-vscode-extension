#include <iostream>
#include <fstream>
#include <string>

#include "../backend/file_helper.h"
#include "../backend/csv_config.h"
#include "../backend/tokenizer.h"
#include "../backend/parser.h"
#include "../backend/binder.h"
#include "../backend/query_executor.h"
#include "../backend/common.h"

int main(int argc, char* argv[]) try {
	constexpr int EXPECTED_ARG_NUMBER = 4;

    if (argc < EXPECTED_ARG_NUMBER) {
        std::cerr << "No input provided" << std::endl;
        return 1;
    }
    
    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];
    std::string queryInput = argv[3];

    // ---------------------------

	std::ifstream inFile;
	FileHelper::open_file_to_read(inputFilePath, inFile);

	std::ofstream outFile;
	FileHelper::create_and_open_file_to_write(outputFilePath, outFile);

    CsvConfig config;
    std::string header = get_header(config.lineDelimeter, inFile);

    Tokenizer tokenier(queryInput);
    auto tokens = tokenier.retrieve_tokens();
    Parser parser(tokens);
    QueryAST ast = parser.parse();
    Binder binder(ast, config, header);
    binder.bind_column_names_to_column_number();

	Selector selector(ast, config, outFile);

	ExternalSorter externalSorter(ast.orderByItems, config.fieldDelimeter, selector);

    QueryExecutor executor(ast, config, inFile, header, std::move(externalSorter), selector);
	executor.run();

	// ---------------------------

    std::cout << "Output written to: " << outputFilePath << std::endl;

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}