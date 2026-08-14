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

    virtual void bind(const std::vector<std::string>& allColumnNames) = 0;
    virtual TokenValue evaluate(const std::string& line, char fieldDelimiter) const = 0;
};

class IdentifierExpression : public Expression {
public:
    std::string name;
    size_t columnIndex = std::string::npos; 

    IdentifierExpression(const std::string& name) : name(name) {}

    void bind(const std::vector<std::string>& allColumnNames) override;
    TokenValue evaluate(const std::string& line, char fieldDelimiter) const override;
};

class LiteralExpression : public Expression {
public:
    TokenValue value;
    TokenType type;

    LiteralExpression(TokenValue value, TokenType type) : value(std::move(value)), type(type) {}

    void bind(const std::vector<std::string>& allColumnNames) override;
    TokenValue evaluate(const std::string& line, char fieldDelimiter) const override;
};

class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    TokenType expressionOperator;

    BinaryExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, TokenType op)
        : left(std::move(left)), right(std::move(right)), expressionOperator(op) {}

    void bind(const std::vector<std::string>& allColumnNames) override;
    TokenValue evaluate(const std::string& line, char fieldDelimiter) const override;

    template <typename T>
    bool compareLeftAndRight(TokenType expressionOperator, T leftOperand, T rightOperand) const;

    bool isDoubleCompare(const TokenValue& leftVal, const TokenValue& rightVal, double& l, double& r) const;
    std::string extractString(const TokenValue& val) const;
};

struct QueryAST {
public:
	bool selectAll = false;
	std::vector<std::string> columnsToSelect;
	std::vector<size_t> indicesOfColumnsToSelect;
	std::unique_ptr<Expression> whereRoot;
	std::optional<int> limit;
};

template<typename T>
inline bool BinaryExpression::compareLeftAndRight(TokenType expressionOperator, T leftOperand, T rightOperand) const
{
    switch (expressionOperator) {
        case TokenType::EQUALS: return leftOperand == rightOperand;
        case TokenType::NOT_EQUALS: return leftOperand != rightOperand;
        case TokenType::LESS_THAN: return leftOperand < rightOperand;
        case TokenType::GREATER_THAN: return leftOperand > rightOperand;
        case TokenType::LESS_EQUAL: return leftOperand <= rightOperand;
        case TokenType::GREATER_EQUAL: return leftOperand >= rightOperand;
        default: return false;
    }
}

#endif // !AST