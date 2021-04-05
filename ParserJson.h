#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

namespace parserjson
{
	enum class Type { STRING, BOOLEAN, ARRAY, NUMBER, OBJECT, NUL };

	class DataJson
	{
	public:
		Type get_type();
		void set_type(Type);
		void add_property(std::string key, DataJson v);
	private:
		Type type;
		std::map<std::string, size_t> mpindex;
		std::vector<std::pair<std::string, DataJson>> properties;
	};

	class ParserJson
	{
	public:
		DataJson Parse(const std::string& str);
	private:
		enum class token_type { UNKNOWN, STRING, NUMBER, BRACKET_OPEN, BRACKET_CLOSE, CROUSH_OPEN, CROUSH_CLOSE,
			BOOLEAN, NUL, COMMA	};
		struct token;
		std::vector<token> tokenize(std::string source);
		int skip_whitespaces(const std::string& source, int i);
		int next_whitespace(const std::string& source, int i);
		Type json_parse(std::vector<token> v, int i, int& r);
	};

}
