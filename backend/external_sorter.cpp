#include <fstream>
#include <algorithm>
#include <queue>

#include "external_sorter.h"
#include "common.h"

bool RowComparator::operator()(const std::string& row1, const std::string& row2) const
{
    for (const auto& item : orderBy_) {
        std::string val1 = get_field_value_by_index(fieldDelimiter_, row1, item.columnIndex);
        std::string val2 = get_field_value_by_index(fieldDelimiter_, row2, item.columnIndex);

        if (val1 == val2) continue;

        // Try compare as numbers first
        double num1 = 0;
        double num2 = 0;

        bool isNum1 = tryParseDouble(val1, num1);
        bool isNum2 = tryParseDouble(val2, num2);

        bool isLess = false;
        if (isNum1 && isNum2) {
            isLess = (num1 < num2);
        } else {
            isLess = (val1 < val2);
        }

        return item.ascending ? isLess : !isLess;
    }

	return false;
}

void RunsFileManager::writeRunToFile(const std::vector<std::string>& run)
{
    std::string tempPath = getTempFilePath();
    std::ofstream out(tempPath);

    for (const auto& line : run) {
        out << line << '\n';
    }

    out.close();
    tempFilePaths_.push_back(tempPath);
}

void ExternalSorter::addRow(std::string&& row)
{
    currentMemoryUsage_ += row.size() + sizeof(std::string);
    buffer_.push_back(std::move(row));

    if (currentMemoryUsage_ >= memoryLimitBytes_) {
        flushRun();
    }
}

void ExternalSorter::mergeRunsAndOutputResult(std::optional<int> limit)
{
    if (!runsFileManager_.hasRuns())
    {
        std::sort(buffer_.begin(), buffer_.end(), comparator_);

        size_t count = 0;
        for (auto& line : buffer_)
        {
			selector_.outputSelectedFields(line);
            ++count;

            if (limit.has_value() && count >= limit) {
                break;
            }
        }
        return;
    }

    if (!buffer_.empty()) {
        flushRun();
    }

    std::vector<std::unique_ptr<std::ifstream>> runFiles;
    for (const auto& path : runsFileManager_.getRunFilePaths())
    {
        runFiles.push_back(std::make_unique<std::ifstream>(path));
    }

    auto heapComp = [this](const HeapNode& a, const HeapNode& b) {
        return comparator_(b.line, a.line);
    };
    std::priority_queue<HeapNode, std::vector<HeapNode>, decltype(heapComp)> minHeap(heapComp);

    // Read 1 line from each run file
    for (size_t i = 0; i < runFiles.size(); ++i)
    {
        std::string line;
        if (std::getline(*runFiles[i], line)) {
            minHeap.push({ std::move(line), i });
        }
    }

    size_t rowsWritten = 0;

    while (!minHeap.empty()) {
        HeapNode top = minHeap.top();
        minHeap.pop();

		selector_.outputSelectedFields(top.line);

        ++rowsWritten;

        if (limit.has_value() && rowsWritten >= limit) {
            break;
        }

        // The next line from the run file that produced the popped row
        std::string nextLine;
        if (std::getline(*runFiles[top.runIndex], nextLine)) {
            minHeap.push({ std::move(nextLine), top.runIndex });
        }
    }
}

void ExternalSorter::flushRun()
{
    if (buffer_.empty()) return;

    std::sort(buffer_.begin(), buffer_.end(), comparator_);

    runsFileManager_.writeRunToFile(buffer_);

    buffer_.clear();
    currentMemoryUsage_ = 0;
}
