#ifndef AST_H_
#define AST_H_

#include <optional>
#include <vector>
#include <string>
#include <memory>

#include "token.h"

class Expression {
public:
	virtual ~Expression() = default;
};

// TODO: add different expressions (literals, comparison, ...)

class BinaryExpression : public Expression {
	std::unique_ptr<Expression> left;
	std::unique_ptr<Expression> right;

	TokenType expressionOperator;
};

struct QueryAST {
public:
	bool selectAll = false;
	std::vector<std::string> columnsToSelect;
	std::vector<size_t> indicesOfColumnsToSelect;
	std::unique_ptr<Expression> whereRoot;
	std::optional<int> limit;
};

#endif // !AST
