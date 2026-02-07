#include <filesystem>
#include "file_helper.h"
#include "common.h"

std::string FileHelper::ensure_index_directory_exists(std::string_view csvFilePath) {
	std::filesystem::path inputPath(csvFilePath);
	auto parentDir = inputPath.parent_path();

	std::string indexDirectoryPath = parentDir.string() + INDEX_FOLDER_NAME;
	std::error_code ec;
	std::filesystem::create_directories(indexDirectoryPath, ec);
	if (ec) {
		throw std::exception(DIRECTORY_FOR_INDEX_CANNOT_CREATE_ERROR);
	}

	return indexDirectoryPath;
}

void FileHelper::create_and_open_file_to_write(std::string_view directoryPath, const std::string& filePath, std::ofstream& outFile)
{
	std::filesystem::path fullPath = std::filesystem::path(directoryPath) / filePath;
	outFile.open(fullPath);

	if (!outFile.good()) {
		throw std::exception(CANNOT_OPEN_FILE_ERROR);
	}
}