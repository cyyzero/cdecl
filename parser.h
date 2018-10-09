#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "tokenizer.h"

struct Symbol
{
    Symbol() = default;
    Symbol(const Symbol&) = default;
    Symbol(Symbol&&) = default;

    Symbol(const std::string& n, const std::string& t)
        : name(n), type(t)
    {
    }

    Symbol(std::string&& n, std::string&& t)
        : name(std::move(n)), type(std::move(t))
    {
    }

    Symbol& operator=(const Symbol&) = default;
    Symbol& operator=(Symbol&&) = default;

    std::string name;
    std::string type;
};

class Parser
{
public:
    Parser(const Tokenizer& tokenizer)
        : m_tokenizer(tokenizer)
    {
    }

    virtual ~Parser() = default;
    Symbol parse();

protected:
    virtual void recursive_parse() = 0;
    Symbol m_symbol;
    Tokenizer m_tokenizer;
};

class Decl_parser : public Parser
{
public:
    Decl_parser(const Tokenizer& tokenizer)
        : Parser(tokenizer)
    {
    }

private:
    void recursive_parse() override;
    void parse_parameter();
};

class Use_parser : public Parser
{
public:
    Use_parser(const Tokenizer& tokenizer)
        : Parser(tokenizer)
    {
    }

private:
    void recursive_parse() override;
};

#endif // PARSER_H
