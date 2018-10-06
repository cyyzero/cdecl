#include <iostream>
#include <string>
#include "tokenizer.h"
#include "parser.h"
int main()
{
    std::string line;
    std::cout <<">>> ";
    while (std::getline(std::cin, line))
    {
        try
        {
            Tokenizer tokenizer(std::move(line));
            Decl_parser parser(tokenizer);
            auto entry = parser.parse();
            std::cout << entry.name << " " << entry.type << std::endl;
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
        std::cout << ">>> ";
    }
}