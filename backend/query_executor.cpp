#include <sstream>

#include "query_executor.h"

void QueryExecutor::outputSelectedFields(std::string& line)
{
    std::string field;
    std::stringstream lineStream(line);
    size_t fieldIndex = 0;
    bool isFirstOutputField = true;

    while (std::getline(lineStream, field, csvConfig_.fieldDelimeter)) {
        // TODO: trim before pushing
        auto it = std::ranges::find(ast_.indicesOfColumnsToSelect, fieldIndex);

        if (ast_.selectAll || it != ast_.indicesOfColumnsToSelect.end()) {

            if (!isFirstOutputField) {
                outputStream_ << csvConfig_.fieldDelimeter;
            }

            outputStream_ << field;

            isFirstOutputField = false;
        }

        ++fieldIndex;
    }

    outputStream_ << csvConfig_.lineDelimeter;
}

void QueryExecutor::run()
{
    std::string line;

    // header
    std::getline(inputStream_, line, csvConfig_.lineDelimeter);
    outputSelectedFields(line);

    while (std::getline(inputStream_, line, csvConfig_.lineDelimeter))
    {
        bool passesWhereClause = true; 

        if (ast_.whereRoot) {
            TokenValue result = ast_.whereRoot->evaluate(line, csvConfig_.fieldDelimeter);
            
            if (std::holds_alternative<bool>(result)) {
                passesWhereClause = std::get<bool>(result);
            } else {
                // If the statement evaluates to something non-boolean 
                passesWhereClause = false;
            }
        }

        if (passesWhereClause) {
            outputSelectedFields(line);

            ++linesInOutput_;

            if (ast_.limit.has_value() && linesInOutput_ >= ast_.limit) {
                break;
            }
        }
    }
}
