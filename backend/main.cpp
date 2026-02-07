#include <iostream>
#include <string>

#include "index_builder.h"

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

    builder.build_index(input);
    
    return 0;
}