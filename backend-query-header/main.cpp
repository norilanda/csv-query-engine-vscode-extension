#include <fstream>
#include <string>
#include <iostream>

#include "../backend/csv_config.h"
#include "../backend/common.h"
#include "../backend/file_helper.h"

int main(int argc, char* argv[]) try {
	constexpr int EXPECTED_ARG_NUMBER = 2;

    if (argc < EXPECTED_ARG_NUMBER) {
        std::cerr << "No input provided" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];

	std::ifstream inFile;
	FileHelper::open_file_to_read(inputFilePath, inFile);

    CsvConfig config;
    std::string header = get_header(config.lineDelimeter, inFile);

	std::cout << header << std::endl;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}