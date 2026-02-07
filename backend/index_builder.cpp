#include <string>
#include <sstream>
#include <format>

#include "index_builder.h"
#include "common.h"
#include "file_statistics.h"
#include "file_helper.h"

// TODO: think of a better structure of index builder, maybe separate logic for bitmap generation, file creation/writing, etc
// TODO: add custom exceptions
void IndexBuilder::build_index(const std::string& inputFilePath) const
{
	std::ifstream fileStream(inputFilePath);

	if (!fileStream.good()) {
		throw std::exception(CANNOT_OPEN_FILE_ERROR);
	}

	size_t columnNumber = get_column_number(fileStream);
	auto positionOfFirstDataLine = fileStream.tellg();

	std::map<std::string, size_t> uniqueValues = FileStatistics::get_unique_values(csvConfig_, fileStream, columnNumber);

	if (uniqueValues.size() > MAXIMUM_COLUMN_CARDINALITY_FOR_BITMAP_INDEX) {
		throw std::exception(COLUMN_CARDINALITY_TOO_LARGE_FOR_BITMAP_ERROR);
	}

	fileStream.clear();
	fileStream.seekg(positionOfFirstDataLine);
	create_and_write_index_to_files(inputFilePath, fileStream, columnNumber, uniqueValues);
}

size_t IndexBuilder::get_column_number(std::ifstream& stream) const
{
	std::string line;
	std::getline(stream, line, csvConfig_.lineDelimeter);

	std::stringstream lineAsStream(line);

	std::string field;
	size_t counter = 0;

	while (std::getline(lineAsStream, field, csvConfig_.fieldDelimeter))
	{
		if (field == columnName_) {
			return counter;
		}

		++counter;
	}

	throw std::exception(COLUMN_WITH_NAME_NOT_EXIST_ERROR);
}

void IndexBuilder::create_and_write_index_to_files(
	const std::string& inputFilePath,
	std::ifstream& stream,
	size_t columnIndex,
	const std::map<std::string, size_t>& map) const
{
	std::string indexDirectoryPath = FileHelper::ensure_index_directory_exists(inputFilePath);

	//create meta file
	std::ofstream metaFileStream;
	FileHelper::create_and_open_file_to_write(
		indexDirectoryPath,
		get_file_name_for_column(STATISTICS_FILE_NAME_PREFIX, columnIndex),
		metaFileStream);

	write_meta_file(map, metaFileStream);
	metaFileStream.close();

	std::map<std::string, std::ofstream> bitStreamMap;
	size_t valueNumber = 0;
	for (auto& [value, count] : map)
	{
		std::ofstream valueBitStream;
		FileHelper::create_and_open_file_to_write(
			indexDirectoryPath,
			get_file_name_for_column(columnIndex, valueNumber),
			valueBitStream);

		bitStreamMap.emplace(value, std::move(valueBitStream));
		++valueNumber;
	}

	std::ofstream lineOffsetStream;
	FileHelper::create_and_open_file_to_write(
		indexDirectoryPath,
		get_file_name_for_column(OFFSET_FILE_NAME_PREFIX, columnIndex),
		lineOffsetStream);

	write_index_to_streams(stream, lineOffsetStream, columnIndex, bitStreamMap);
}

void IndexBuilder::write_index_to_streams(
	std::istream& inputStream,
	std::ofstream& lineOffsetStream,
	size_t columnIndex,
	std::map<std::string, std::ofstream>& bitStreamMap) const
{

    std::string line;
	size_t lineStartPos = inputStream.tellg();

	while (std::getline(inputStream, line, csvConfig_.lineDelimeter)) 
	{
        if (line.empty()) {
			lineStartPos = inputStream.tellg();
            continue;
        }

		lineOffsetStream << lineStartPos;
		lineStartPos = inputStream.tellg();

        std::string fieldValue = get_field_value_by_index(csvConfig_.fieldDelimeter, line, columnIndex);
		
	// TODO: create and utilize bit writer
		for (auto& [value, bitStream] : bitStreamMap) {
			if (fieldValue == value) {
				bitStream << 1;
			} else {
				bitStream << 0;
			}
		}
	}
}

void IndexBuilder::write_meta_file(const std::map<std::string, size_t>& map, std::ostream& outStream) const
{
	for (auto it = map.begin(); it != map.end(); ++it)
	{
		auto& [value, count] = *it;

		outStream << value << "\n";
		outStream << count << "\n";
	}
}