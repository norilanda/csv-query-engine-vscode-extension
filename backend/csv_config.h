#ifndef CSV_CONFIG_H_
#define CSV_CONFIG_H_

#include <string_view>

constexpr char DEFAULT_LINE_DELIM = '\n';
constexpr char DEFAULT_FIELD_DELIM = ',';

struct CsvConfig {
public:
	const char lineDelimiter;
	const char fieldDelimiter;

	CsvConfig(char lineDel = DEFAULT_LINE_DELIM, char fieldDel = DEFAULT_FIELD_DELIM)
		: lineDelimiter(lineDel), fieldDelimiter(fieldDel) { };
};

#endif // !CSV_CONFIG_H_
