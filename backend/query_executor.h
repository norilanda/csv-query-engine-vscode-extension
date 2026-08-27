#ifndef QUERY_EXECUTOR_H_
#define QUERY_EXECUTOR_H_

#include <iostream>

#include "ast.h"
#include "csv_config.h"
#include "selector.h"
#include "external_sorter.h"

class QueryExecutor {
private:
	QueryAST& ast_;
	CsvConfig csvConfig_;
	std::istream& inputStream_;
	std::string& header_;
	ExternalSorter sorter_;
	Selector selector_;
	size_t linesInOutput_ = 0;

public:
	QueryExecutor(
		QueryAST& ast,
		CsvConfig csvConfig,
		std::istream& inputStream,
		std::string& header,
		ExternalSorter&& externalSorter,
		Selector selector)
		: ast_(ast), csvConfig_(csvConfig), inputStream_(inputStream), header_(header), sorter_(std::move(externalSorter)), selector_(selector) { }

	void run();

private:
	void runWithoutOrderBy();
	void runWithOrderBy();
	bool passesWhereClause(const std::string& line);
};

#endif
