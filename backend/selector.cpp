#include <sstream>

#include "selector.h"

void Selector::outputSelectedFields(std::string_view line)
{
    if (ast_.selectAll) {
        outputStream_ << line << csvConfig_.lineDelimiter;
        return;
    }

    fieldsCache_.clear();

    size_t start = 0;
    for (size_t i = 0; i <= line.size(); ++i) {
        if (i == line.size() || line[i] == csvConfig_.fieldDelimiter) {
            fieldsCache_.push_back(line.substr(start, i - start));
            start = i + 1;
        }
    }

    for (size_t i = 0; i < ast_.indicesOfColumnsToSelect.size(); ++i) {
        if (i > 0) {
            outputStream_ << csvConfig_.fieldDelimiter;
        }

        size_t targetIndex = ast_.indicesOfColumnsToSelect[i];
        if (targetIndex < fieldsCache_.size()) {
            outputStream_ << fieldsCache_[targetIndex];
        }
    }

    outputStream_ << csvConfig_.lineDelimiter;
}
