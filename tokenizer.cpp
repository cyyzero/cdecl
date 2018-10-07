#include <string>
#include <unordered_set>
#include "tokenizer.h"

const Tokenizer::Token& Tokenizer::get()
{
    return m_token;
}

void Tokenizer::next()
{
    static std::unordered_set<std::string> types = {
        "void", "int", "short", "float", "double", "char"
    };

    if (m_current != m_line.end())
    {
        decltype(m_current) last_pos;
        while (m_current != m_line.end() && isspace(*m_current))
            ++m_current;
        if (isdigit(*m_current))
        {
            last_pos = m_current;
            while (m_current != m_line.end() && isdigit(*m_current))
            {
                ++m_current;
            }
            m_token = {Number, std::string(last_pos, m_current)};
            return;
        }
        else if (isalpha(*m_current))
        {
            last_pos = m_current;
            while (m_current != m_line.end() && (isalnum(*m_current) || *m_current == '_'))
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
            return;
        }
        else
        {
            last_pos = m_current;
            ++m_current;
            switch (*last_pos)
            {
            case '*':
                m_token = {Star, "*"};
                return;
            case '&':
                m_token = {Address, "&"};
                return;
            case ',':
                m_token = {Comma, ","};
                return;
            case '[':
                m_token = {LSquare, "["};
                return;
            case ']':
                m_token = {RSquare, "]"};
                return;
            case '(':
                m_token = {LBraket, "("};
                return;
            case ')':
                m_token = {RBraket, ")"};
                return;
            default:
                throw std::logic_error("Bad token");
            }
        }
    }
    m_token = {End, "end"};
}

void Tokenizer::reset()
{
    m_current = m_line.begin();
    next();
}