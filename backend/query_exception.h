#ifndef QUERY_EXCEPTION_H_
#define QUERY_EXCEPTION_H_

#include <stdexcept>

constexpr char QUERY_EMPTY_ERROR[] = "Query is empty";
constexpr char INVALID_STRING_LITERAL_ERROR[] = "Invalid string literal";
constexpr char INVALID_NUMBER_ERROR[] = "Invalid number";
constexpr char INVALID_DECIMAL_POINT_ERROR[] = "Invalid decimal point found";
constexpr char MISSING_SELECT_ERROR[] = "Expect SELECT at start";
constexpr char EXPECT_COLUMN_NAME_ERROR[] = "Expect column name";
constexpr char MISSING_LIMIT_ERROR[] = "Expect LIMIT keyword";
constexpr char MISSING_NUMBER_LITERAL_ERROR[] = "Expect number literal";
constexpr char EXPECT_INTEGER_LITERAL_ERROR[] = "Expect integer number";
constexpr char MISSING_WHERE_ERROR[] = "Expect WHERE keyword";
constexpr char INVALID_ORDER_BY_ERROR[] = "Expect ORDER BY keyword";
constexpr char EXPECT_EXPRESSION_ERROR[] = "Expect expression";
constexpr char LIMIT_SHOULD_BE_POSITIVE_ERROR[] = "Limit should be positive";
constexpr char EXPECT_END_OF_QUERY_ERROR[] = "Expect end of query";

constexpr char INVALID_COLUMN_NAME_ERROR[] = "Column name is not present in the header file";

class QueryException : public std::runtime_error {
public:
	QueryException(const std::string& message)
		: std::runtime_error(message) { }	
};

// ------------------------------------------------------

class TokenizerException : public std::runtime_error {
public:
	TokenizerException(const std::string& message)
		: std::runtime_error(message) { }	
};

// ------------------------------------------------------

class ParserException : public std::runtime_error {
public:
	ParserException(const std::string& message)
		: std::runtime_error(message) { }	
};

// ------------------------------------------------------

class BinderException : public std::runtime_error {
public:
	BinderException(const std::string& message)
		: std::runtime_error(message) { }	
};

#endif // !QUERY_EXCEPTION_H_
