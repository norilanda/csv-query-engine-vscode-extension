#ifndef EXTERNAL_SORTER_H_
#define EXTERNAL_SORTER_H_

#include <vector>
#include <filesystem>

#include "ast.h"
#include "selector.h"

/** 
 * Compares two rows based on the ORDER BY rules.
 */
class RowComparator
{
private:
	const std::vector<OrderByItem>& orderBy_;
	char fieldDelimiter_;

public:
	/** 
	 * Constructs a RowComparator.
	 */
	RowComparator(const std::vector<OrderByItem>& orderBy, char fieldDelimiter)
		: orderBy_(orderBy), fieldDelimiter_(fieldDelimiter) {}

	/** 
	 * Compares string row1 and string row2.
	 */
	bool operator()(const std::string& row1, const std::string& row2) const;
};

/** 
 * Node that tracks a line from a specific run index in a min-heap.
 */
struct HeapNode {
    std::string line;
    size_t runIndex;
};

/** 
 * Manages run files during external sorting.
 */
class RunsFileManager
{
private:
    std::string tempDirectory_;
    std::vector<std::string> tempFilePaths_;

public:
	RunsFileManager(const RunsFileManager&) = delete;
	RunsFileManager& operator=(const RunsFileManager&) = delete;

	/** 
	 * Constructs a RunsFileManager with a temporary directory.
	 */
	RunsFileManager(const std::string& tempDirectory)
		: tempDirectory_(tempDirectory) {
		std::filesystem::create_directories(tempDirectory_);
	}

	/** 
	 * Move constructor.
	 */
	RunsFileManager(RunsFileManager&& other) noexcept
		: tempDirectory_(std::move(other.tempDirectory_)),
		  tempFilePaths_(std::move(other.tempFilePaths_)) { }

	/** 
	 * Destructor that cleans up temp files and directory.
	 */
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

	/** 
	 * Writes a sorted run to a temporary file.
	 */
	void writeRunToFile(const std::vector<std::string>& run);
	/** 
	 * True if there are any recorded run files.
	 */
	bool hasRuns() const {
		return !tempFilePaths_.empty();
	}
	/** 
	 * Gets the vector of all run file paths.
	 */
	const std::vector<std::string>& getRunFilePaths() const {
		return tempFilePaths_;
	}

private:
    std::string getTempFilePath() const {
        return tempDirectory_ + "/run_" + std::to_string(tempFilePaths_.size()) + ".tmp";
	}
};

/** 
 * Class responsible for performing external sort of rows based on ORDER BY directives and memory constraints.
 */
class ExternalSorter
{
private:
	static constexpr size_t DEFAULT_MEMORY_LIMIT_BYTES = 100 * 1024 * 1024; // 100 MB
	size_t currentMemoryUsage_ = 0;
	std::vector<std::string> buffer_;
	RunsFileManager runsFileManager_;
	RowComparator comparator_;
	Selector selector_;
	size_t memoryLimitBytes_;

public:
	ExternalSorter(const ExternalSorter&) = delete;
	ExternalSorter& operator=(const ExternalSorter&) = delete;

	/** 
	 * Move constructor.
	 */
	ExternalSorter(ExternalSorter&&) = default;

	/** 
	 * Constructs an ExternalSorter.
	 */
	ExternalSorter(
		const std::vector<OrderByItem>& orderBy,
		char fieldDelimiter,
		Selector selector,
		size_t memory_limit_bytes = DEFAULT_MEMORY_LIMIT_BYTES,
		const std::string& tempDirectory = "./external-sort/")
		: runsFileManager_(tempDirectory), comparator_(orderBy, fieldDelimiter), selector_(selector), memoryLimitBytes_(memory_limit_bytes) { }

	/** 
	 * Adds a row of data. Will trigger a flush to active run if memory threshold limits are met.
	 */
	void addRow(std::string&& row);
	/** 
	 * Merges all stored runs and streams them into limit/result sets.
	 */
	void mergeRunsAndOutputResult(std::optional<size_t> limit);

private:
    void flushRun();
};

#endif // !EXTERNAL_SORTER_H_
