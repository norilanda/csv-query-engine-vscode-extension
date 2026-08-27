#ifndef BINDER_H_
#define BINDER_H_

#include "ast.h"
#include "csv_config.h"

class Binder {
private:
	QueryAST& ast_;
	CsvConfig csvConfig_;
	std::string header_;


public:
	Binder(QueryAST& ast, CsvConfig& csvConfig, const std::string& header)
		: ast_(ast), csvConfig_(csvConfig), header_(header) { }

	void bind_column_names_to_column_number();


private:
	void bind_select(std::vector<std::string>& allColumnNames);
	void bind_order_by(std::vector<std::string>& allColumnNames);
};

#endif // !BINDER_H_
