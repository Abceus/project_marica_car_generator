#pragma once

#include <memory>
#include <string>
#include <list>

enum class ASETokenType {
    None,
    Value,
    Key,
    String,
    BlockStart,
    BlockEnd,
    Skip
};

struct ASEToken {
    std::string data;
    ASETokenType type = ASETokenType::None;

    void clear();
};

class ASETokenizer {
public:
    void tokenization(const std::string& text);

    ASEToken getCurrentTokent() const;
    ASEToken nextToken();
    bool isLastToken() const;
    ASEToken peekNextToken(size_t offset = 1) const;
    void markSkip(size_t offset = 1);
private:
    std::list<ASEToken> tokens;
    decltype(tokens)::iterator currentTokenIt;
};