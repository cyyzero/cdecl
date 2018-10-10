#include <string>
#include <stdexcept>
#include <unordered_map>
#include "tokenizer.h"
#include "parser.h"

std::unordered_map<std::string, std::string> g_symbols;

void human_readable(const std::string& type)
{
    // std::string readable_type;
    auto cur = type.begin();
    while (cur != type.end())
    {
        // "void", "int", "short", "float", "double", "char"
        switch (*cur)
        {
        case 'A':
        {
            ++cur;
            auto last = cur;
            // ++cur;
            std::cout << "array ";
            while (isdigit(*last))
            {
                ++cur;
            }
            std::cout << std::string(last, cur) << " of ";
            ++cur;
            break;
        }

        case '*':
            ++cur;
            std::cout << "pointer to ";
            break;

        case 'F':
        case 'v':
            ++cur;
            std::cout << "void ";
            break;

        case 'i':
            ++cur;
           std::cout << "int ";
           break;

        case 's':
            ++cur;
            std::cout << "short ";
            break;

        case 'f':
            ++cur;
            std::cout << "float ";
            break;

        case 'd':
            
        case 'c':
        default:
            throw std::logic_error("fuck");
        }
    }
}

// class Parser
Symbol Parser::parse()
{
    recursive_parse();
    return m_symbol;
}

// class Decl_parser
void Decl_parser::recursive_parse()
{
    auto token = m_tokenizer.get();

    if (token.first == Tokenizer::End)
        return;

    switch (token.first)
    {
    case Tokenizer::Identifier:
        m_symbol.name = token.second;
        m_tokenizer.next();
        break;
    case Tokenizer::LBraket:
        m_tokenizer.next();
        recursive_parse();
        if (m_tokenizer.get().first != Tokenizer::RBraket)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();
        break;
    case Tokenizer::Star:
        m_tokenizer.next();
        recursive_parse();
        if (m_tokenizer.get().first == Tokenizer::LSquare)
        {
            recursive_parse();
        }
        m_symbol.type += "*";
        break;
    case Tokenizer::Type:
        m_tokenizer.next();
        recursive_parse();
        if (m_tokenizer.get().first == Tokenizer::LSquare)
        {
            recursive_parse();
        }
        m_symbol.type += token.second[0];
        break;
    default:
        break;
    }

    token = m_tokenizer.get();
    switch (token.first)
    {
    case Tokenizer::LSquare:
        m_tokenizer.next();
        token = m_tokenizer.get();
        if (token.first != Tokenizer::Number)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();
        if (m_tokenizer.get().first != Tokenizer::RSquare)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();
        m_symbol.type = (m_symbol.type + "A" + token.second + "_");
        break;
    case Tokenizer::LBraket:
        m_symbol.type += "F(";
        m_tokenizer.next();
        parse_parameter();
        m_tokenizer.next();
        m_symbol.type += ")->";
        break;
    default:
        break;
    }
}

void Decl_parser::parse_parameter()
{
    while (m_tokenizer.get().first != Tokenizer::RBraket)
    {
        recursive_parse();
        if (m_tokenizer.get().first == Tokenizer::Comma)
        {
            m_tokenizer.next();
        }

        if (m_tokenizer.get().first == Tokenizer::End)
        {
            throw std::logic_error("syntax error");
        }
    }
}

void Use_parser::recursive_parse()
{
    auto token = m_tokenizer.get();

    if (token.first == Tokenizer::End)
    {
        return;
    }

    switch (token.first)
    {
    case Tokenizer::Identifier:
    {
        auto it = g_symbols.find(token.second);
        if (it == g_symbols.end())
        {
            throw std::logic_error("Unknown identifier");
        }
        m_symbol = Symbol(it->first, it->second);
        m_tokenizer.next();
        break;
    }
    case Tokenizer::LBraket:
        m_tokenizer.next();
        recursive_parse();
        if (m_tokenizer.get().first != Tokenizer::RBraket)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();
        break;
    case Tokenizer::Star:
        m_tokenizer.next();
        recursive_parse();
        // if type is not pointer
        if (m_symbol.type[0] != '*')
        {
            std::string err = std::move(m_symbol.name) + " can't be dereference";
            throw std::logic_error(err.c_str());
        }
        m_symbol.name.insert(0, "*");
        m_symbol.type.erase(0, 1);
        break;
    case Tokenizer::Address:
        m_tokenizer.next();
        recursive_parse();
        m_symbol.name.insert(0, "&");
        m_symbol.type.insert(0, "*");
        break;
    case Tokenizer::Type:
        throw std::logic_error("syntax");
    default:
        break;
    }

    token = m_tokenizer.get();
    if (token.first ==  Tokenizer::LSquare)
    {
        m_tokenizer.next();
        auto number = m_tokenizer.get();
        if (number.first != Tokenizer::Number)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();
        if (m_tokenizer.get().first != Tokenizer::RSquare)
        {
            throw std::logic_error("syntax error");
        }
        m_tokenizer.next();

        if (m_symbol.type[0] == 'A')       // type is array
        {
            auto it = m_symbol.type.begin() + 1;
            while (isdigit(*it))
            {
                ++it;
            }
            auto index = stoul(number.second);
            auto length = stoul(std::string(m_symbol.type.begin() + 1, it));
            if (index >= length)
            {
                throw std::logic_error("index out of range");
            }
            m_symbol.type.erase(m_symbol.type.begin(), it+1);
            m_symbol.name = m_symbol.name + "[" + number.second + "]";
        }
        else if (m_symbol.type[0] == '*')  // type is pointer
        {
            m_symbol.type.erase(0, 1);
            m_symbol.name = m_symbol.name + "[" + number.second + "]";
        }
        else
        {
            throw std::logic_error("syntax");
        }
    }
    recursive_parse();
}