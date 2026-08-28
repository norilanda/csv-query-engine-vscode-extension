#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <vector>
#include <string_view>

#include "token.h"

/** 
 * Splits an input query string into a sequence of Tokens.
 */
class Tokenizer {
private:
	std::string_view input_;

	std::vector<Token> tokens_;
	size_t currentPos_;
	bool digitFound_;
	bool letterFound_;
	bool minusFound_;
	size_t tokenStart_;


public:
	/** 
	 * Constructs a Tokenizer from a query string.
	 */
	Tokenizer(std::string_view input)
		: input_(input),
		currentPos_(0),
		digitFound_(false),
		letterFound_(false),
		minusFound_(false),
		tokenStart_(std::string::npos) { }

	/** 
	 * Scans the string and returns all lexical tokens.
	 */
	std::vector<Token> retrieve_tokens();

private:
	void process_digit();
	void process_letter();
	void process_minus();
	void process_decimal_point();
	void process_string_literal();
	bool process_if_current_char_is_operator_token();
	void process_token_if_needed();
	void process_letters_token(std::string_view tokenValue);
	void process_digits_token(std::string_view tokenValue);
};


#endif // !TOKENIZER_H_
