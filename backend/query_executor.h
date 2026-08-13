#ifndef QUERY_EXECUTOR_H_
#define QUERY_EXECUTOR_H_

#include <iostream>

#include "ast.h"

class QueryExecutor {
private:
	QueryAST ast_;
	std::ostream& outputStream_;
	size_t linesInOutput_ = 0;

public:
	QueryExecutor(QueryAST&& ast, std::ostream& outputStream)
		: ast_(std::move(ast)), outputStream_(outputStream) { }

};

#endif
