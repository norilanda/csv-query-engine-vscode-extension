#include <algorithm>
#include <string>

#include "ast.h"
#include "query_exception.h"
#include "common.h"

// --- Literal Expression ---
void LiteralExpression::bind(const std::vector<std::string>& allColumnNames) { }

TokenValue LiteralExpression::evaluate(const std::string& line, char fieldDelimiter) const {
    return value;
}

// --- Identifier Expression ---
void IdentifierExpression::bind(const std::vector<std::string>& allColumnNames) {
    auto it = std::ranges::find(allColumnNames, name);

    if (it != allColumnNames.end())
    {
        columnIndex = std::distance(allColumnNames.begin(), it);
    } else {
        throw BinderException(INVALID_COLUMN_NAME_ERROR);
    }
}

TokenValue IdentifierExpression::evaluate(const std::string& line, char fieldDelimiter) const {
    return get_field_value_by_index(fieldDelimiter, line, columnIndex);
}

// --- Binary Expression ---
void BinaryExpression::bind(const std::vector<std::string>& allColumnNames) {
    left->bind(allColumnNames);
    right->bind(allColumnNames);
}

TokenValue BinaryExpression::evaluate(const std::string& line, char fieldDelimiter) const {
    TokenValue leftVal = left->evaluate(line, fieldDelimiter);
    TokenValue rightVal = right->evaluate(line, fieldDelimiter);

    // 1. Handle Logical AND / OR
    if (expressionOperator == TokenType::AND || expressionOperator == TokenType::OR) {
        bool leftBool = std::holds_alternative<bool>(leftVal) && std::get<bool>(leftVal);
        bool rightBool = std::holds_alternative<bool>(rightVal) && std::get<bool>(rightVal);
        
        if (expressionOperator == TokenType::AND) {
            return leftBool && rightBool;
        } else {
            return leftBool || rightBool;
        }
    }

    // 2. Handle Comparisons (Try comparing as doubles first if possible)
    double ld = 0.0, rd = 0.0;

    if (isDoubleCompare(leftVal, rightVal, ld, rd))
    {
        return compareLeftAndRight<double>(expressionOperator, ld, rd);
    }

    // 3. Fallback to String comparison
    std::string ls = extractString(leftVal);
    std::string rs = extractString(rightVal);

    return compareLeftAndRight<std::string>(expressionOperator, ls, rs);

}

bool BinaryExpression::isDoubleCompare(const TokenValue& leftVal, const TokenValue& rightVal, double& l, double& r) const
{
    // Coerce to double if one side is double and the other side is a string (e.g., from CSV Identifier)
    if (std::holds_alternative<double>(leftVal) && std::holds_alternative<double>(rightVal)) {
        l = std::get<double>(leftVal);
        r = std::get<double>(rightVal);
        return true;
    } else if (std::holds_alternative<std::string>(leftVal) && std::holds_alternative<double>(rightVal)) {
        try {
            l = std::stod(std::get<std::string>(leftVal));
            r = std::get<double>(rightVal);
            return true;
        } catch (...) { /* Not a number, fallback to string compare */ }
    } else if (std::holds_alternative<double>(leftVal) && std::holds_alternative<std::string>(rightVal)) {
        try {
            l = std::get<double>(leftVal);
            r = std::stod(std::get<std::string>(rightVal));
            return true;
        } catch (...) { /* Not a number, fallback to string compare */ }
    }

    return false;
}

std::string BinaryExpression::extractString(const TokenValue& val) const
{
    if (std::holds_alternative<std::string>(val))
        return std::get<std::string>(val);

    if (std::holds_alternative<double>(val))
        return std::to_string(std::get<double>(val));

    if (std::holds_alternative<bool>(val))
        return std::get<bool>(val) ? TRUE_LITERAL : FALSE_LITERAL;

    return "";
}
