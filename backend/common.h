#ifndef COMMON_H_
#define COMMON_H_

#include <string>

// errors
constexpr char CANNOT_OPEN_FILE_ERROR[] = "Cannot open file";
constexpr char COLUMN_WITH_NAME_NOT_EXIST_ERROR[] = "Column with a provided name does not exist";
constexpr char COLUMN_NOT_EXISTS[] = "Column does not exist";
constexpr char COLUMN_CARDINALITY_TOO_LARGE_FOR_BITMAP_ERROR[] = "Column cardinality is too large to build the bitmap index";

constexpr char DIRECTORY_FOR_INDEX_CANNOT_CREATE_ERROR[] = "Cannot create directory for an index file";

// constants
constexpr size_t MAXIMUM_COLUMN_CARDINALITY_FOR_BITMAP_INDEX = 3;
constexpr char INDEX_FOLDER_NAME[] = ".csvqidx";
constexpr char STATISTICS_FILE_NAME_PREFIX[] = "statistics";
constexpr char OFFSET_FILE_NAME_PREFIX[] = "offset";

// f-ns
// TODO: helper?
std::string get_field_value_by_index(char fieldDelimeter, const std::string& line, size_t columnIndex);

inline std::string get_file_name_for_column(const char* nameStart, size_t columnIndex) {
	return nameStart + std::to_string(columnIndex);
}

inline std::string get_file_name_for_column(size_t columnIndex, size_t valueIndex) {
	return std::to_string(columnIndex) + "_" + std::to_string(valueIndex);
}

#endif // !COMMON_H_
