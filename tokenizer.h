#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>

class Tokenizer
{
public:
    enum Token_kind: uint8_t
    {
        Number, Identifier, Type,
        Address, LSquare, RSquare, LBraket, RBraket, Star, Comma,
        End
    };
    using Token = std::pair<Token_kind, std::string>;
public:
    Tokenizer(const std::string& line)
        : m_line(line)
    {
        reset();
    }

    Tokenizer(std::string&& line)
        : m_line(std::move(line))
    {
        reset();
    }

    Tokenizer(const Tokenizer& tokenizer)
        : m_line(tokenizer.m_line)
    {
        reset();
    }

    Tokenizer(Tokenizer&& tokenizer)
        : m_line(std::move(tokenizer.m_line))
    {
        reset();
    }

    ~Tokenizer() = default;

    Token get();
    void next();
    void reset();
private:
    //std::istream& m_file;
    Token m_token;
    std::string m_line;
    std::string::iterator m_current;
};

#endif