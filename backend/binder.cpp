#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "binder.h"
#include "query_exception.h"

void Binder::bind_column_names_to_column_number()
{
    std::vector<std::string> allColumnNames;
    std::stringstream stream(header_);
    std::string column;

    while (std::getline(stream, column, csvConfig_.fieldDelimeter)) {
        // TODO: trim before pushing
        allColumnNames.push_back(column);
    }

    bind_select(allColumnNames);

    if (ast_.whereRoot) {
        ast_.whereRoot->bind(allColumnNames);
    }
}

void Binder::bind_select(std::vector<std::string>& allColumnNames)
{
    for (auto& col : ast_.columnsToSelect)
    {
        auto it = std::ranges::find(allColumnNames, col);
        if (it != allColumnNames.end())
        {
            auto index = std::distance(allColumnNames.begin(), it);
            ast_.indicesOfColumnsToSelect.push_back(index);
        }
        else {
            throw BinderException(INVALID_COLUMN_NAME_ERROR);
        }
    }
}

