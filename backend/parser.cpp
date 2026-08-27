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

    if (peek().type == TokenType::ORDER)
    {
        parseOrderBy(ast);
    }

    if (peek().type != TokenType::END_OF_FILE)
    {
        throw ParserException(EXPECT_END_OF_QUERY_ERROR);
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

void Parser::parseLimit(QueryAST& ast)
{
    consume(TokenType::LIMIT, MISSING_LIMIT_ERROR);

    double limitLiteral = std::get<double>(
        consume(TokenType::NUMBER, MISSING_NUMBER_LITERAL_ERROR).literal
    );

    if (limitLiteral != std::floor(limitLiteral)) {
        throw ParserException(EXPECT_INTEGER_LITERAL_ERROR);
    }

    if (limitLiteral < 0) {
        throw ParserException(LIMIT_SHOULD_BE_POSITIVE_ERROR);
	}

    ast.limit = limitLiteral;
}

void Parser::parseWhere(QueryAST& ast)
{
    consume(TokenType::WHERE, MISSING_WHERE_ERROR);
    ast.whereRoot = parseExpression();
}

// Routes to the lowest precedence operator (OR)
std::unique_ptr<Expression> Parser::parseExpression()
{
    return parseOr();
}

// Parses logical OR (Lowest precedence)
std::unique_ptr<Expression> Parser::parseOr()
{
    auto expr = parseAnd();

    while (peek().type == TokenType::OR)
    {
        TokenType op = move_next().type;
        auto right = parseAnd();
        expr = std::make_unique<BinaryExpression>(std::move(expr), std::move(right), op);
    }

    return expr;
}

// Parses logical AND (Medium precedence)
std::unique_ptr<Expression> Parser::parseAnd()
{
    auto expr = parseComparison();

    while (peek().type == TokenType::AND)
    {
        TokenType op = move_next().type;
        auto right = parseComparison();
        expr = std::make_unique<BinaryExpression>(std::move(expr), std::move(right), op);
    }

    return expr;
}

// Parses conditions like =, !=, >, < (Highest precedence operator)
std::unique_ptr<Expression> Parser::parseComparison()
{
    auto expr = parsePrimary();

    while (peek().type == TokenType::EQUALS || 
           peek().type == TokenType::NOT_EQUALS ||
           peek().type == TokenType::LESS_THAN ||
           peek().type == TokenType::GREATER_THAN ||
           peek().type == TokenType::LESS_EQUAL ||
           peek().type == TokenType::GREATER_EQUAL)
    {
        TokenType op = move_next().type;
        auto right = parsePrimary();
        expr = std::make_unique<BinaryExpression>(std::move(expr), std::move(right), op);
    }

    return expr;
}

// Parses raw base values (Identifiers and Literals)
std::unique_ptr<Expression> Parser::parsePrimary()
{
    Token token = move_next();
    
    if (token.type == TokenType::IDENTIFIER) {
        return std::make_unique<IdentifierExpression>(token.lexeme);
    }
    
    if (token.type == TokenType::STRING_LITERAL || 
        token.type == TokenType::NUMBER || 
        token.type == TokenType::BOOLEAN) {
        return std::make_unique<LiteralExpression>(token.literal, token.type);
    }
    
    throw ParserException(EXPECT_EXPRESSION_ERROR);
}

void Parser::parseOrderBy(QueryAST& ast)
{
   consume(TokenType::ORDER, INVALID_ORDER_BY_ERROR);
   consume(TokenType::BY, INVALID_ORDER_BY_ERROR);

   auto consumeOrderByItem = [&]()
   {
       std::string columnName = consume(TokenType::IDENTIFIER, EXPECT_COLUMN_NAME_ERROR).lexeme;
       bool ascending = true;

       if (peek().type == TokenType::ASC)
       {
           move_next();
       }
       else if (peek().type == TokenType::DESC)
       {
           move_next();
           ascending = false;
       }

       ast.orderByItems.emplace_back(std::move(columnName), ascending);
   };

   consumeOrderByItem();

   while (peek().type == TokenType::COMMA)
   {
	   move_next();

       consumeOrderByItem();
   }
}