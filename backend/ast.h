#ifndef AST_H_
#define AST_H_

#include <optional>
#include <vector>
#include <string>
#include <memory>

#include "token.h"

/** 
 * Base class for expression nodes in the AST.
 */
class Expression {
public:
    virtual ~Expression() = default;

    /** 
     * Binds the expression.
     */
    virtual void bind(const std::vector<std::string>& allColumnNames) = 0;
    /** 
     * Evaluates the expression.
     */
    virtual TokenValue evaluate(const std::string& line, char fieldDelimiter) const = 0;
};

/** 
 * Expression node representing an identifier/column.
 */
class IdentifierExpression : public Expression {
public:
    std::string name;
    size_t columnIndex = std::string::npos; 

    /** 
     * Constructs an IdentifierExpression.
     */
    IdentifierExpression(const std::string& name) : name(name) {}

    /** 
     * Binds the identifier to a column index.
     */
    void bind(const std::vector<std::string>& allColumnNames) override;
    /** 
     * Evaluates the column value from a row.
     */
    TokenValue evaluate(const std::string& line, char fieldDelimiter) const override;
};

/** 
 * Expression node representing a literal value.
 */
class LiteralExpression : public Expression {
public:
    TokenValue value;
    TokenType type;

    /** 
     * Constructs a LiteralExpression.
     */
    LiteralExpression(TokenValue value, TokenType type) : value(std::move(value)), type(type) {}

    /** 
     * Binds the literal expression.
     */
    void bind(const std::vector<std::string>& allColumnNames) override;
    /** 
     * Evaluates the literal value.
     */
    TokenValue evaluate(const std::string& line, char fieldDelimiter) const override;
};

/** 
 * Expression node representing a binary operation.
 */
class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    TokenType expressionOperator;

    /** 
     * Constructs a BinaryExpression.
     */
    BinaryExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, TokenType op)
        : left(std::move(left)), right(std::move(right)), expressionOperator(op) {}

    /** 
     * Binds the binary expression.
     */
    void bind(const std::vector<std::string>& allColumnNames) override;
    /** 
     * Evaluates the binary expression.
     */
    TokenValue evaluate(const std::string& line, char fieldDelimiter) const override;

    /** 
     * Compares the left and right operands.
     */
    template <typename T>
    bool compareLeftAndRight(TokenType expressionOperator, T leftOperand, T rightOperand) const;

    /** 
     * Checks if it is a double comparison.
     */
    bool isDoubleCompare(const TokenValue& leftVal, const TokenValue& rightVal, double& l, double& r) const;
    /** 
     * Extracts a string from a token value.
     */
    std::string extractString(const TokenValue& val) const;
};

// ----------------------------------------------------------------

/** 
 * Structure representing an item in the ORDER BY clause.
 */
struct OrderByItem {
public:
	size_t columnIndex;
	bool ascending;
	std::string columnName;

	/** 
	 * Constructs an OrderByItem.
	 */
	OrderByItem(std::string&& colName, bool asc) 
		: columnIndex(std::string::npos), columnName(colName), ascending(asc) {}
};

// ----------------------------------------------------------------

/** 
 * Abstract Syntax Tree for a parsed query.
 */
struct QueryAST {
public:
	bool selectAll = false;
	std::vector<std::string> columnsToSelect;
	std::vector<size_t> indicesOfColumnsToSelect;
	std::unique_ptr<Expression> whereRoot;
	std::vector<OrderByItem> orderByItems;
	std::optional<size_t> limit;
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