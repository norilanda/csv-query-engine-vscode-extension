#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <variant>

constexpr char STRING_LITERAL_INDICATOR = '\'';
constexpr char DECIMAL_POINT = '.';
constexpr char UNDERSCORE = '_';

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
	END_OF_FILE,
};

constexpr char TRUE_LITERAL[] = "TRUE";
constexpr char FALSE_LITERAL[] = "FALSE";

inline std::map<std::string, TokenType> keywordTokenTypeMap {
	{ "SELECT", TokenType::SELECT },
	{ "WHERE", TokenType::WHERE },
	{ "LIMIT", TokenType::LIMIT },
	{ "AND", TokenType::AND },
	{ "OR", TokenType::OR },
	{ TRUE_LITERAL, TokenType::BOOLEAN },
	{ FALSE_LITERAL, TokenType::BOOLEAN },
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

using TokenValue = std::variant<std::monostate, std::string, double, bool>;

class Token {
public:
	const TokenType type;
	const std::string lexeme;
	const TokenValue literal; // TODO: check if literal works correctly

	Token(TokenType inputType, std::string&& inputLexeme, TokenValue&& inputLiteral)
		: type(inputType), lexeme(std::move(inputLexeme)), literal(std::move(inputLiteral)) { }

	Token(TokenType inputType, const std::string& inputLexeme, const TokenValue& inputLiteral)
		: type(inputType), lexeme(inputLexeme), literal(inputLiteral) { }

	Token(TokenType inputType, std::string&& inputLexeme)
		: Token(inputType, std::move(inputLexeme), std::monostate {}) { }

	Token(TokenType inputType, const std::string& inputLexeme)
		: Token(inputType, inputLexeme, std::monostate {}) { }
};

#endif // !TOKEN_H_
