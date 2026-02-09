#include <iostream>
#include <string>

#include "index_builder.h"
#include "tokenizer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No input provided" << std::endl;
        return 1;
    }
    
    std::string input = argv[1];
    
    // Your CSV processing logic would go here
    // For now, simple string manipulation
    std::string output = "processed " + input;

    // Output the result (extension will read from stdout)
    std::cout << output << std::endl;

    std::string colName = "weather";
    IndexBuilder builder(colName);

    //builder.build_index(input);

    // ---------------------------

    std::string queryInput = "SELECT weather WHERE weather = 'sun'";
    std::string queryInput1 = R"(SELECT * WHERE weather = 'sun' LIMIT 1)";

    Tokenizer tokenier(queryInput);
    auto tokens = tokenier.retrieve_tokens();

    Tokenizer tokenier1(queryInput1);
    auto tokens1 = tokenier1.retrieve_tokens();
    
    return 0;
}