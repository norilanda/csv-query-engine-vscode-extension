#include <sstream>

#include "query_executor.h"
#include "external_sorter.h"

void QueryExecutor::run()
{
    selector_.outputSelectedFields(header_);

    if (ast_.orderByItems.empty())
    {
        runWithoutOrderBy();
    }
    else
    {
        runWithOrderBy();
	}
}

void QueryExecutor::runWithoutOrderBy()
{
    std::string line;

    while (std::getline(inputStream_, line, csvConfig_.lineDelimeter))
    {
        if (passesWhereClause(line)) {
            selector_.outputSelectedFields(line);

            ++linesInOutput_;

            if (ast_.limit.has_value() && linesInOutput_ >= ast_.limit) {
                break;
            }
        }
    }
}

void QueryExecutor::runWithOrderBy()
{
    std::string line;

    while (std::getline(inputStream_, line, csvConfig_.lineDelimeter))
    {
        if (passesWhereClause(line)) {
            sorter_.addRow(std::move(line));
        }
    }

    sorter_.mergeRunsAndOutputResult(ast_.limit);
}

bool QueryExecutor::passesWhereClause(const std::string& line)
{
    bool passesWhereClause = true; 

    if (ast_.whereRoot) {
        TokenValue result = ast_.whereRoot->evaluate(line, csvConfig_.fieldDelimeter);
            
        if (std::holds_alternative<bool>(result)) {
            passesWhereClause = std::get<bool>(result);
        } else {
            passesWhereClause = false;
        }
    }

    return passesWhereClause;
}
