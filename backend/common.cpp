#include <sstream>
#include <charconv>
#include <cctype>

#include "common.h"

std::string_view get_field_view_by_index(char fieldDelimiter, std::string_view line, size_t targetIndex)
{
    size_t start = 0;
    size_t currentIndex = 0;

    for (size_t i = 0; i <= line.size(); ++i)
    {
        if (i == line.size() || line[i] == fieldDelimiter)
        {
            if (currentIndex == targetIndex) {
                return line.substr(start, i - start);
            }
            currentIndex++;
            start = i + 1;
        }
    }
    throw std::runtime_error(COLUMN_NOT_EXISTS);
}

std::string get_header(char lineDelimiter, std::istream& inputStream)
{
    std::string header;
    std::getline(inputStream, header, lineDelimiter);
    return header;
}

std::string trim(const std::string& str)
{
    std::string_view sv = trim(std::string_view(str));
    return std::string(sv);
}

std::string_view trim(std::string_view str)
{
	constexpr char whitespace[] = " \t\n\r\f\v";

	size_t start = str.find_first_not_of(whitespace);

	if (start == std::string::npos) {
		return "";
	}
	size_t end = str.find_last_not_of(whitespace);
	return str.substr(start, end - start + 1);
}

bool tryParseDouble(std::string_view str, double& outVal)
{
	str = trim(str);

    if (str.empty()) {
        return false;
    }

    const char* first = str.data();
    const char* last = str.data() + str.size();
    
    auto [ptr, ec] = std::from_chars(first, last, outVal);

    return (ec == std::errc() && ptr == last);
}
