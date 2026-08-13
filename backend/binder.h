#ifndef BINDER_H_
#define BINDER_H_

#include "ast.h"

class Binder {
private:
	QueryAST& ast_;
	std::string header_;

public:
	Binder(QueryAST& ast, const std::string& header)
		: ast_(ast), header_(header) { }

	QueryAST& bind_column_names_to_column_number();
};

#endif // !BINDER_H_
