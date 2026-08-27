#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <string_view>

// errors
constexpr char CANNOT_OPEN_FILE_ERROR[] = "Cannot open file";
constexpr char COLUMN_WITH_NAME_NOT_EXIST_ERROR[] = "Column with a provided name does not exist";
constexpr char COLUMN_NOT_EXISTS[] = "Column does not exist";

std::string_view get_field_view_by_index(char fieldDelimiter, std::string_view line, size_t targetIndex);
std::string get_header(char lineDelimeter, std::istream& inputStream);

std::string trim(const std::string& str);
std::string_view trim(std::string_view str);

bool tryParseDouble(std::string_view str, double& outVal);

#endif // !COMMON_H_
