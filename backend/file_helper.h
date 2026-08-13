#ifndef FILE_HELPER_H_
#define FILE_HELPER_H_

#include <string>
#include <string_view>
#include <fstream>

class FileHelper {
public:
	static void create_and_open_file_to_write(std::string_view directoryPath, const std::string& filePath, std::ofstream& outFile);
};

#endif // !FILE_HELPER_H_
