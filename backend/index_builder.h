#ifndef INDEX_BUILDER_H_
#define INDEX_BUILDER_H_

#include <string>
#include <string_view>
#include <fstream>
#include <set>
#include <exception>
#include <map>

#include "csv_config.h"
#include "common.h"

class IndexBuilder {
private:
	std::string_view columnName_;
	CsvConfig csvConfig_;

public:
	IndexBuilder(std::string_view columnName, CsvConfig csvConfig = CsvConfig())
		: columnName_(columnName), csvConfig_(csvConfig) { };

	void build_index(const std::string& inputFilePath) const;

private:
	size_t get_column_number(std::ifstream& stream) const;

	void create_and_write_index_to_files(
		const std::string& inputFilePath,
		std::ifstream& stream,
		size_t columnIndex,
		const std::map<std::string, size_t>& map) const;

	void write_index_to_streams(
		std::istream& inputStream,
		std::ofstream& lineOffsetStream,
		size_t columnIndex,
		std::map<std::string, std::ofstream>& bitStreamMap) const;

	void write_meta_file(const std::map<std::string, size_t>& map, std::ostream& outStream) const;
};

#endif // !INDEX_BUILDER_H_
