#ifndef EXTERNAL_SORTER_H_
#define EXTERNAL_SORTER_H_

#include <vector>
#include <filesystem>

#include "ast.h"
#include "selector.h"

class RowComparator
{
private:
    const std::vector<OrderByItem>& orderBy_;
    char fieldDelimiter_;

public:
	RowComparator(const std::vector<OrderByItem>& orderBy, char fieldDelimiter)
		: orderBy_(orderBy), fieldDelimiter_(fieldDelimiter) {}

	bool operator()(const std::string& row1, const std::string& row2) const;
};

struct HeapNode {
    std::string line;
    size_t runIndex;
};

class RunsFileManager
{
private:
    std::string tempDirectory_;
    std::vector<std::string> tempFilePaths_;

public:
    RunsFileManager(const RunsFileManager&) = delete;
    RunsFileManager& operator=(const RunsFileManager&) = delete;

	RunsFileManager(const std::string& tempDirectory)
		: tempDirectory_(tempDirectory) {
		std::filesystem::create_directories(tempDirectory_);
	}

	RunsFileManager(RunsFileManager&& other) noexcept
		: tempDirectory_(std::move(other.tempDirectory_)),
		  tempFilePaths_(std::move(other.tempFilePaths_)) { }

	~RunsFileManager() {
		if (tempDirectory_.empty()) return;

		for (const auto& f : tempFilePaths_)
        {
            std::error_code ec;
            std::filesystem::remove(f, ec);
        }

        std::error_code ec;
		std::filesystem::remove(tempDirectory_, ec);
    }

    void writeRunToFile(const std::vector<std::string>& run);
    bool hasRuns() const {
        return !tempFilePaths_.empty();
	}
    const std::vector<std::string>& getRunFilePaths() const {
		return tempFilePaths_;
	}

private:
    std::string getTempFilePath() const {
        return tempDirectory_ + "/run_" + std::to_string(tempFilePaths_.size()) + ".tmp";
	}
};

class ExternalSorter
{
private:
    static constexpr size_t DEFAULT_MEMORY_LIMIT_BYTES = 50 * 1024 * 1024; // 50 MB
    size_t currentMemoryUsage_ = 0;
    std::vector<std::string> buffer_;
	RunsFileManager runsFileManager_;
    RowComparator comparator_;
	Selector selector_;
    size_t memoryLimitBytes_;

public:
    ExternalSorter(const ExternalSorter&) = delete;
    ExternalSorter& operator=(const ExternalSorter&) = delete;

    ExternalSorter(ExternalSorter&&) = default;

    ExternalSorter(
        const std::vector<OrderByItem>& orderBy,
        char fieldDelimiter,
        Selector selector,
		size_t memory_limit_bytes = DEFAULT_MEMORY_LIMIT_BYTES,
        const std::string& tempDirectory = "./external-sort/")
		: runsFileManager_(tempDirectory), comparator_(orderBy, fieldDelimiter), selector_(selector), memoryLimitBytes_(memory_limit_bytes) { }

    void addRow(std::string&& row);
    void mergeRunsAndOutputResult(std::optional<int> limit);

private:
    void flushRun();
};

#endif // !EXTERNAL_SORTER_H_
