#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <map>
#include <vector>
#include <tuple>

constexpr char STRING_LITERAL_INDICATOR = '\'';
constexpr char DECIMAL_POINT = '.';

enum class TokenType
{
	// Keywords
	SELECT,
	WHERE,
	LIMIT,
	AND,
	OR,

	// Literals/Identifiers
	IDENTIFIER,
	STRING_LITERAL,
	NUMBER,
	BOOLEAN,

	// Operators
    EQUALS,          // =
    NOT_EQUALS,      // !=
    LESS_THAN,       // <
    GREATER_THAN,    // >
    LESS_EQUAL,      // <=
    GREATER_EQUAL,   // >=
    ASTERISK,        // *

	// Punctuation
    COMMA,           // ,
};

inline std::map<std::string, TokenType> keywordTokenTypeMap {
	{ "SELECT", TokenType::SELECT },
	{ "WHERE", TokenType::WHERE },
	{ "LIMIT", TokenType::LIMIT },
	{ "AND", TokenType::AND },
	{ "OR", TokenType::OR },
	{ "TRUE", TokenType::BOOLEAN },
	{ "FALSE", TokenType::BOOLEAN },
};

inline std::vector<std::tuple<std::string, TokenType>> operatorsTokenTypeList {
	// Ordered by length
	{ "!=", TokenType::NOT_EQUALS },
	{ "<=", TokenType::LESS_EQUAL },
	{ ">=", TokenType::GREATER_EQUAL },

	{ ",", TokenType::COMMA },
	{ "*", TokenType::ASTERISK },
	{ "=", TokenType::EQUALS },
	{ "<", TokenType::LESS_THAN },
	{ ">", TokenType::GREATER_THAN },
};

class Token {
public:
	const TokenType type;
	const std::string value;

	Token(TokenType inputType, std::string&& inputValue)
		: type(inputType), value(std::move(inputValue)) { }

	Token(TokenType inputType, const std::string& inputValue)
		: type(inputType), value(inputValue) { }
};

#endif // !TOKEN_H_
