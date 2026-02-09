#ifndef QUERY_EXCEPTION_H_
#define QUERY_EXCEPTION_H_

#include <stdexcept>

constexpr char QUERY_EMPTY_ERROR[] = "Query is empty";
constexpr char INVALID_STRING_LITERAL_ERROR[] = "Invalid string literal";
constexpr char INVALID_NUMBER_ERROR[] = "Invalid number";
constexpr char INVALID_DECIMAL_POINT_ERROR[] = "Invalid decimal point found";


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


#endif // !QUERY_EXCEPTION_H_
