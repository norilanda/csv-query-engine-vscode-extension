#ifndef COMMON_H_
#define COMMON_H_

#include <string>

// errors
constexpr char CANNOT_OPEN_FILE_ERROR[] = "Cannot open file";
constexpr char COLUMN_WITH_NAME_NOT_EXIST_ERROR[] = "Column with a provided name does not exist";
constexpr char COLUMN_NOT_EXISTS[] = "Column does not exist";

// f-ns
// TODO: helper?
std::string get_field_value_by_index(char fieldDelimeter, const std::string& line, size_t columnIndex);
std::string get_header(char lineDelimeter, std::istream& inputStream);

#endif // !COMMON_H_
