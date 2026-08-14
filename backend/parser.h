#ifndef PARSER_H_
#define PARSER_H_

#include <vector>

#include "token.h"
#include "ast.h"
#include "query_exception.h"

class Parser {
private:
	std::vector<Token> tokens_;
	size_t current_ = 0;

public:
	Parser(const std::vector<Token>& tokens)
		: tokens_(tokens) { }

	QueryAST parse();

private:
	const Token& peek() const { return tokens_[current_]; }

	Token& move_next()
	{
        if (peek().type != TokenType::END_OF_FILE) {
			current_++;
		}

        return tokens_[current_ - 1];
    }

    Token& consume(TokenType type, const std::string& message)
	{
        if (peek().type == type) {
			return move_next();
		}

        throw ParserException(message);
    }

	void parseSelect(QueryAST& ast);
	void parseWhere(QueryAST& ast);
	void parseLimit(QueryAST& ast);

    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseOr();
    std::unique_ptr<Expression> parseAnd();
    std::unique_ptr<Expression> parseComparison();
    std::unique_ptr<Expression> parsePrimary();
};

#endif // !PARSER_H_
