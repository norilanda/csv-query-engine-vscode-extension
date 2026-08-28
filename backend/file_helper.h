#ifndef FILE_HELPER_H_
#define FILE_HELPER_H_

#include <string>
#include <string_view>
#include <fstream>

/** 
 * Helper class for file operations.
 */
class FileHelper {
public:
	/** 
	 * Opens a file for reading.
	 */
	static void open_file_to_read(const std::string& filePath, std::ifstream& inFile);
	/** 
	 * Creates and opens a file for writing.
	 */
	static void create_and_open_file_to_write(const std::string& filePath, std::ofstream& outFile);
};

#endif // !FILE_HELPER_H_
