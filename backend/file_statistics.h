#ifndef FILE_STATISTICS_H_
#define FILE_STATISTICS_H_

#include <istream>
#include <map>
#include <string>

#include "csv_config.h"

class FileStatistics {
public:
	static std::map<std::string, size_t> get_unique_values(CsvConfig csvConfig, std::istream& stream, size_t columnIndex);
};

#endif // !FILE_STATISTICS_H_
