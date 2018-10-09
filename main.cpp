#include <iostream>
#include <string>
#include <unordered_map>
#include "tokenizer.h"
#include "parser.h"

extern std::unordered_map<std::string, std::string> g_symbols;

int main()
{
    std::string line;
    std::cout <<">>> ";
    while (std::getline(std::cin, line))
    {
        try
        {
            Tokenizer tokenizer(std::move(line));
            if (tokenizer.get().first == Tokenizer::Type)
            {
                Decl_parser parser(tokenizer);
                auto entry = parser.parse();
                g_symbols[entry.name] = entry.type;
                std::cout << entry.name << " " << entry.type << std::endl;
            }
            else
            {
                Use_parser parser(tokenizer);
                auto entry = parser.parse();
                std::cout << entry.name << " " << entry.type << std::endl;
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
        std::cout << ">>> ";
    }
}