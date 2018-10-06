#include <string>
#include <unordered_map>
#include "tokenizer.h"

Token Tokenizer::get()
{
    return m_token;
}

void Tokenizer::next()
{
    static std::unordered_map<std::string> types = {
        "void", "int", "short", "float", "double", "char"
    };

    auto end = m_line.end();

    while (m_current != m_line.end())
    {
        decltype(m_current) last_pos;
        if (isdigit(*m_current))
        {
            last_pos = m_current;
            while (m_current < m_line.end() && isdigit(*m_current))
            {
                ++m_current;
            }
            m_token = {Number, sd::string(last_pos, m_current)};
        }
        else if (isalpha(*m_current))
        {
            last_pos = m_current;
            while (m_current < m_line.end() && isalpha(*m_current))
            {
                ++m_current;
            }
            std::string type = std::string(last_pos, m_current);
            if (types.find(type) != types.end())
            {
                m_token = {Type, std::move(type)};
            }
            else
            {
                m_token = {Identifier, std::move(type)};
            }
        }
        else
        {
            switch (*m_current)
            {
            case '*':
                m_token = {Star, "*"};
                break;
            case '&':
                m_token = {Address, "&"};
                break;
            case ',':
                m_token = {Comma, ","};
                break;
            case '[':
                m_token = {LSquare, "["};
                break;
            case ']':
                m_token = {RSquare, "]"};
                break;
            case '(':
                m_token = {LBraket, "("};
                break;
            case ')':
                m_token = {RBraket, ")"};
            default:
                throw std::logic_error("Bad token");
            }
        }
    }
}

void Tokenizer::reset()
{
    m_current = m_line.begin();
    next();
}