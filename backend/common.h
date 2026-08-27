#ifndef COMMON_H_
#define COMMON_H_

#include <string>

// errors
constexpr char CANNOT_OPEN_FILE_ERROR[] = "Cannot open file";
constexpr char COLUMN_WITH_NAME_NOT_EXIST_ERROR[] = "Column with a provided name does not exist";
constexpr char COLUMN_NOT_EXISTS[] = "Column does not exist";

std::string get_field_value_by_index(char fieldDelimeter, const std::string& line, size_t columnIndex);
std::string get_header(char lineDelimeter, std::istream& inputStream);

std::string trim(const std::string& str);
std::string_view trim(std::string_view str);

bool tryParseDouble(std::string_view str, double& outVal);

#endif // !COMMON_H_
