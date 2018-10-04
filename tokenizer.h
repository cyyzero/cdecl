#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>

class Tokenizer
{
public:
    Tokenizer(const std::string& code)
        : m_code(code) { }
    ~Tokenizer() = default;

private:
    const std::string& m_code;
};

#endif