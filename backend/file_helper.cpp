#include <filesystem>
#include "file_helper.h"
#include "common.h"

void FileHelper::open_file_to_read(const std::string& filePath, std::ifstream& inFile)
{
	inFile.open(filePath);

	if (!inFile.good()) {
		throw std::exception(CANNOT_OPEN_FILE_ERROR);
	}
}

void FileHelper::create_and_open_file_to_write(const std::string& filePath, std::ofstream& outFile)
{
	outFile.open(filePath);

	if (!outFile.good()) {
		throw std::exception(CANNOT_OPEN_FILE_ERROR);
	}
}