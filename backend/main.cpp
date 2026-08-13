#include <iostream>
#include <string>

#include "tokenizer.h"
#include "parser.h"

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

    // ---------------------------

    std::string queryInput = "SELECT weather WHERE weather = 'sun'";
    std::string queryInput1 = "SELECT * LIMIT 1";

    Tokenizer tokenier(queryInput);
    auto tokens = tokenier.retrieve_tokens();
    Parser parser(tokens);
    auto ast = parser.parse();

    Tokenizer tokenier1(queryInput1);
    auto tokens1 = tokenier1.retrieve_tokens();
    Parser parser1(tokens1);
    auto ast1 = parser1.parse();
    
    return 0;
}