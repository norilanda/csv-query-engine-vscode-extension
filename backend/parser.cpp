#include <cmath>

#include "parser.h"

QueryAST Parser::parse()
{
    QueryAST ast;
    parseSelect(ast);

    if (peek().type == TokenType::WHERE)
    {
        parseWhere(ast);
    }

    if (peek().type == TokenType::LIMIT)
    {
        parseLimit(ast);
    }

    return ast;
}

void Parser::parseSelect(QueryAST& ast)
{
    consume(TokenType::SELECT, MISSING_SELECT_ERROR);

    if (peek().type == TokenType::ASTERISK)
    {
        ast.selectAll = true;
        move_next();
    }
    else
    {
        std::string columnName = consume(TokenType::IDENTIFIER, EXPECT_COLUMN_NAME_ERROR).lexeme;
        ast.columnsToSelect.push_back(std::move(columnName));

        while (peek().type == TokenType::COMMA)
        {
            move_next();
            columnName = consume(TokenType::IDENTIFIER, EXPECT_COLUMN_NAME_ERROR).lexeme;
            ast.columnsToSelect.push_back(std::move(columnName));
        }
    }
}

void Parser::parseWhere(QueryAST& ast)
{
    // TODO
}

void Parser::parseLimit(QueryAST& ast)
{
    consume(TokenType::LIMIT, MISSING_LIMIT_ERROR);

    double limitLiteral = std::get<double>(
        consume(TokenType::NUMBER, MISSING_NUMBER_LITERAL_ERROR).literal
    );

    if (limitLiteral != std::floor(limitLiteral)) {
        throw ParserException(EXPECT_INTEGER_LITERAL_ERROR);
    }

    ast.limit = limitLiteral;
}
