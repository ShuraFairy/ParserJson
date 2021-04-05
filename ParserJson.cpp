#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include "ParserJson.h"

namespace parserjson
{
	Type DataJson::get_type() 
	{
		return type;
	}

	void DataJson::set_type(Type tp) 
	{
		type = tp;
	}

	void DataJson::add_property(std::string key, DataJson v) 
	{
		mpindex[key] = properties.size();
		properties.push_back(make_pair(key, v));
	}


	int ParserJson::skip_whitespaces(const std::string& source, int i) 
	{
		while (i < static_cast<int>(source.length())) 
		{
			if (!isspace(source[i])) 
				return i;
			i++;
		}
		return -1;
	}

	int ParserJson::next_whitespace(const std::string& source, int i) 
	{
		while (i < static_cast<int>(source.length())) 
		{
			if (source[i] == '"') {
				i++;
				while (i < static_cast<int>(source.length()) && (source[i] != '"'))// || source[i - 1] == '\\')) 
				{
					i++;
				}
			}
			/*if (source[i] == '\'') 
			{
				i++;
				while (i < (int)source.length() && (source[i] != '\'' || source[i - 1] == '\\')) i++;
			}*/
			if (isspace(source[i])) return i;
			i++;
		}
		return static_cast<int>(source.length());
	}

	struct ParserJson::token 
	{
		std::string value;
		token_type type;
		token(std::string value = "", token_type type = token_type::UNKNOWN) : value(value), type(type) 
		{ }
	};

	std::vector<ParserJson::token> ParserJson::tokenize(std::string source)
	{
		source += "";
		std::vector<token> tokens;
		int index = skip_whitespaces(source, 0);

		while (index >= 0) {
			int next = next_whitespace(source, index);
			std::string str = source.substr(index, next - index);

			size_t count = 0;
			while (count < str.length()) 
			{
				if (str[count] == '"') 
				{
					size_t temp_count = count + 1;
					while (temp_count < str.length() && (str[temp_count] != '"'))// || str[temp_count - 1] == '\\'))
					{
						temp_count++;
					}
					tokens.push_back(token(str.substr(count + 1, temp_count - count - 1), token_type::STRING));
					count = temp_count + 1;
					continue;
				}
			}
		}

		return tokens;
	}

	Type ParserJson::json_parse(std::vector<token> v, int i, int& r) {
		DataJson current;		
		if (v[i].type == token_type::CROUSH_OPEN) {
			current.set_type(Type::OBJECT);
			int k = i + 1;
			while (v[k].type != token_type::CROUSH_CLOSE) {
				std::string key = v[k].value;
				k += 2; // k+1 should be ':'
				int j = k;
				Type vv = json_parse(v, k, j);
				current.add_property(key, vv);
				k = j;
				if (v[k].type == token_type::COMMA) k++;
			}
			r = k + 1;
			return current;
		}
		if (v[i].type == BRACKET_OPEN) {
			current.set_type(ARRAY);
			int k = i + 1;
			while (v[k].type != BRACKET_CLOSE) {
				int j = k;
				jValue vv = json_parse(v, k, j);
				current.add_element(vv);
				k = j;
				if (v[k].type == COMMA) k++;
			}
			r = k + 1;
			return current;
		}
		if (v[i].type == NUMBER) {
			current.set_type(Type::NUMBER);
			current.set_string(v[i].value);
			r = i + 1;
			return current;
		}
		if (v[i].type == STRING) {
			current.set_type(JSTRING);
			current.set_string(v[i].value);
			r = i + 1;
			return current;
		}
		if (v[i].type == BOOLEAN) {
			current.set_type(BOOLEAN);
			current.set_string(v[i].value);
			r = i + 1;
			return current;
		}
		if (v[i].type == NUL) {
			current.set_type(JNULL);
			current.set_string("null");
			r = i + 1;
			return current;
		}
		return current;
	}

	DataJson ParserJson::Parse(const std::string& str) {
		int k;
		return json_parse(tokenize(str), 0, k);
	}
}

