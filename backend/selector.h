#ifndef SELECTOR_H_
#define SELECTOR_H_

#include <iostream>
#include <string_view>
#include <vector>

#include "ast.h"
#include "csv_config.h"

class Selector
{
private:
	QueryAST& ast_;
	CsvConfig csvConfig_;
	std::ostream& outputStream_;
	std::vector<std::string_view> fieldsCache_;

public:
	Selector(QueryAST& ast, CsvConfig csvConfig, std::ostream& outputStream)
		: ast_(ast), csvConfig_(csvConfig), outputStream_(outputStream) {}

	void outputSelectedFields(std::string_view line);
};

#endif // !SELECTOR_H_
