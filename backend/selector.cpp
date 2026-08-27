#include <sstream>

#include "selector.h"

void Selector::outputSelectedFields(std::string& line)
{
    std::string field;
    std::stringstream lineStream(line);
    size_t fieldIndex = 0;
    bool isFirstOutputField = true;

    while (std::getline(lineStream, field, csvConfig_.fieldDelimeter)) {
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
