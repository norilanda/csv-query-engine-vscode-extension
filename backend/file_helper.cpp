#include <filesystem>
#include "file_helper.h"
#include "common.h"

void FileHelper::open_file_to_read(std::string_view directoryPath, const std::string& filePath, std::ifstream& inFile)
{
	std::filesystem::path fullPath = std::filesystem::path(directoryPath) / filePath;
	inFile.open(fullPath);

	if (!inFile.good()) {
		throw std::exception(CANNOT_OPEN_FILE_ERROR);
	}
}

void FileHelper::create_and_open_file_to_write(std::string_view directoryPath, const std::string& filePath, std::ofstream& outFile)
{
	std::filesystem::path fullPath = std::filesystem::path(directoryPath) / filePath;
	outFile.open(fullPath);

	if (!outFile.good()) {
		throw std::exception(CANNOT_OPEN_FILE_ERROR);
	}
}