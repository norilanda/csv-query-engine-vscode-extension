#ifndef QUERY_EXECUTOR_H_
#define QUERY_EXECUTOR_H_

#include <iostream>

#include "ast.h"
#include "csv_config.h"

class QueryExecutor {
private:
	QueryAST& ast_;
	CsvConfig csvConfig_;
	std::istream& inputStream_;
	std::ostream& outputStream_;
	size_t linesInOutput_ = 0;

public:
	QueryExecutor(QueryAST& ast, CsvConfig csvConfig, std::istream& inputStream, std::ostream& outputStream)
		: ast_(ast), csvConfig_(csvConfig), inputStream_(inputStream), outputStream_(outputStream) { }

	void run();

private:
	void outputSelectedFields(std::string& line);
};

#endif
