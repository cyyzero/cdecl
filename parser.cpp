#include <stdexcept>
#include "parser.h"

// class Parser
Symbol Parser::parse()
{
    recursive_parse();
    return m_entry;
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
        m_entry.name = token.second;
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
        m_entry.type += "*";
        break;
    case Tokenizer::Type:
        m_tokenizer.next();
        recursive_parse();
        m_entry.type += token.second[0];
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
        m_entry.type = (m_entry.type + "A" + token.second + "_");
        break;
    case Tokenizer::LBraket:
        m_entry.type += "F(";
        m_tokenizer.next();
        parse_parameter();
        m_tokenizer.next();
        m_entry.type += ")->";
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

}