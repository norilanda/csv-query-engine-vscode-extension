#ifndef BINDER_H_
#define BINDER_H_

#include "ast.h"
#include "csv_config.h"

/** 
 * Class responsible for binding abstract syntax tree identifiers to actual column indices.
 */
class Binder {
private:
	QueryAST& ast_;
	CsvConfig csvConfig_;
	std::string header_;


public:
	/** 
	 * Constructs a new Binder.
	 */
	Binder(QueryAST& ast, CsvConfig& csvConfig, const std::string& header)
		: ast_(ast), csvConfig_(csvConfig), header_(header) { }

	/** 
	 * Binds column names in the AST to their corresponding column indices.
	 */
	void bind_column_names_to_column_number();


private:
	void bind_select(std::vector<std::string>& allColumnNames);
	void bind_order_by(std::vector<std::string>& allColumnNames);
};

#endif // !BINDER_H_
