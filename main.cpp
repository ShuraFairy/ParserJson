//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <string>
#include "ParserJson.h"


int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    std::ifstream in("test.json");
    std::string str{};
    std::string temp;

    while (getline(in, temp))
    {
        str += temp;
    }
    parserjson::ParserJson parser;
    parserjson::DataJson js = parser.Parse(str);

    return EXIT_SUCCESS;
}
