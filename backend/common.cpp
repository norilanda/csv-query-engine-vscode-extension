#include <sstream>
#include <charconv>
#include <cctype>

#include "common.h"

// TODO: maybe create some helper?
std::string get_field_value_by_index(char fieldDelimeter, const std::string& line, size_t columnIndex)
{
    std::string field;
    std::stringstream lineAsStream(line);

    size_t currentFieldNumber = 0;
    while (std::getline(lineAsStream, field, fieldDelimeter))
    {
        if(currentFieldNumber == columnIndex)
        {
            break;
        }

        ++currentFieldNumber;
    }
        
    if (currentFieldNumber != columnIndex) {
        throw std::exception(COLUMN_NOT_EXISTS);
    }

    return field;
}

std::string get_header(char lineDelimeter, std::istream& inputStream)
{
    std::string header;
    std::getline(inputStream, header, lineDelimeter);
    return header;
}

std::string trim(const std::string& str)
{
	return trim(std::string_view(str)).data();
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
