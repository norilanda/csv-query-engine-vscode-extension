#include <sstream>
#include <exception>

#include "file_statistics.h"
#include "common.h"

std::map<std::string, size_t> FileStatistics::get_unique_values(CsvConfig csvConfig, std::istream& stream, size_t columnIndex)
{
    // TODO: throw memory exception if needed?
    std::map<std::string, size_t> values;

    std::string line;
    while (std::getline(stream, line, csvConfig.lineDelimeter))
    {
        if (line.empty()) {
            continue;
        }

        std::string field = get_field_value_by_index(csvConfig.fieldDelimeter, line, columnIndex);

        values[field]++;
    }

    return values;
}
