#include <sstream>

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
