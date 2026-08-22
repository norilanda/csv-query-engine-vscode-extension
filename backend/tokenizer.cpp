#include <string_view>
#include <algorithm>
#include <cctype>

#include "tokenizer.h"
#include "query_exception.h"

std::vector<Token> Tokenizer::retrieve_tokens()
{
    if (input_.empty()) {
        throw QueryException(QUERY_EMPTY_ERROR);
    }

    while (currentPos_ != input_.size())
    {
		char currentChar = input_[currentPos_];

        if (std::isdigit(currentChar))
        {
            process_digit();
            continue;
        }
        else if (currentChar == DECIMAL_POINT)
        {
            process_decimal_point();
            continue;
        }
        else if (std::isalpha(currentChar) || currentChar == UNDERSCORE)
        {
            process_letter();
            continue;
        }

        process_token_if_needed();

        if (process_if_current_char_is_operator_token())
        { }
        else if (currentChar == STRING_LITERAL_INDICATOR)
        {
            process_string_literal();
        }
        else
        {
            ++currentPos_;
        }
    }

    process_token_if_needed();

    tokens_.emplace_back(TokenType::END_OF_FILE, "");
    return tokens_;
}

inline void Tokenizer::process_digit()
{
    digitFound_ = true;

    if (tokenStart_ == std::string::npos) {
        tokenStart_ = currentPos_;
    }

    ++currentPos_;
}

inline void Tokenizer::process_letter()
{
    letterFound_ = true;

    if (tokenStart_ == std::string::npos) {
        tokenStart_ = currentPos_;
    }

    ++currentPos_;
}

inline void Tokenizer::process_decimal_point()
{
    if (tokenStart_ == std::string::npos) {
        throw TokenizerException(INVALID_DECIMAL_POINT_ERROR);
    }

    ++currentPos_;
}


inline void Tokenizer::process_string_literal()
{
    size_t nextCharPos = currentPos_ + 1;
    size_t stringLiteralEndPosition = input_.find_first_of(STRING_LITERAL_INDICATOR, nextCharPos);

    if (stringLiteralEndPosition == std::string::npos) {
        throw TokenizerException(INVALID_STRING_LITERAL_ERROR);
    }

    std::string_view stringLiteralValue = input_.substr(nextCharPos, stringLiteralEndPosition - nextCharPos);
    tokens_.emplace_back(TokenType::STRING_LITERAL, std::string(stringLiteralValue), std::string(stringLiteralValue));

    currentPos_ = stringLiteralEndPosition + 1;
}

bool Tokenizer::process_if_current_char_is_operator_token()
{
    auto check_if_starts_with_operator_lambda = [this] (std::tuple<std::string, TokenType> operatorTokenTypePair) -> bool {
        std::string_view substring = input_.substr(currentPos_);
        return substring.starts_with(std::get<0>(operatorTokenTypePair));
    };

    if (auto it = std::ranges::find_if(operatorsTokenTypeList, check_if_starts_with_operator_lambda);
        it != operatorsTokenTypeList.end())
    {
        std::string& token = std::get<0>(*it);
        TokenType type = std::get<1>(*it);
            
        tokens_.emplace_back(type, token);
            
        currentPos_ += token.size();

        return true;
    }

    return false;
}

inline void Tokenizer::process_token_if_needed()
{
    if (tokenStart_ == std::string::npos)
    {
        return;
    }

    std::string_view tokenValue = input_.substr(tokenStart_, currentPos_ - tokenStart_);

    if (digitFound_ && letterFound_)
    {
        tokens_.emplace_back(TokenType::IDENTIFIER, std::string(tokenValue));
    } 
    else if (letterFound_)
    {
        process_letters_token(tokenValue);
    }
    else
    {
        process_digits_token(tokenValue);
    }

    tokenStart_ = std::string::npos;
    digitFound_ = false;
    letterFound_ = false;
}

inline void Tokenizer::process_letters_token(std::string_view tokenValue)
{
    std::string uppercaseTokenValue(tokenValue);
    std::ranges::transform(uppercaseTokenValue, uppercaseTokenValue.begin(), [] (unsigned char c) {
        return std::toupper(c);
    });

    auto keywordIt = keywordTokenTypeMap.find(uppercaseTokenValue);
    if (keywordIt != keywordTokenTypeMap.end())
    {
        if (keywordIt->first == TRUE_LITERAL)
        {
            tokens_.emplace_back(keywordIt->second, std::string(tokenValue), true);
        }
        else if (keywordIt->first == FALSE_LITERAL)
        {
            tokens_.emplace_back(keywordIt->second, std::string(tokenValue), false);
        }
        else {
            tokens_.emplace_back(keywordIt->second, std::string(tokenValue), std::string(tokenValue));
        }
    }
    else {
        tokens_.emplace_back(TokenType::IDENTIFIER, std::string(tokenValue)); 
    }
}

inline void Tokenizer::process_digits_token(std::string_view tokenValue)
{
    size_t pos;
    double number = std::stod(std::string(tokenValue), &pos);

    if (pos < tokenValue.size()) {
        throw TokenizerException(INVALID_NUMBER_ERROR);
    }

    tokens_.emplace_back(TokenType::NUMBER, std::string(tokenValue), number); 
}
