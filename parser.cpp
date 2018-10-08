#include <string>
#include <stdexcept>
#include <unordered_map>
#include "tokenizer.h"
#include "parser.h"

std::unordered_map<std::string, std::string> symbols;

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
        m_symbol.type += "*";
        break;
    case Tokenizer::Type:
        m_tokenizer.next();
        recursive_parse();
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
        auto it = symbols.find(token.second);
        if (it == symbols.end())
        {
            throw std::logic_error("Unknown identifier");
        }
        m_symbol = Symbol(it.first, it.second);
        m_tokenizer.next();
        break;
    }
    case Tokenizer::LBraket:
        m_tokenizer.next();
        recursive_parse();
        if (m_tokenizer.get().next != Tokenizer::RBraket)
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
    switch (token.first)
    {
    case Tokenizer::LSquare:
    {
        if (m_symbol.type[0] != 'A')
        {
            std::string err = m_symbol.name + " is not an array";
            throw std::logic_error(err.c_str());
        }
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
    }
    }
}