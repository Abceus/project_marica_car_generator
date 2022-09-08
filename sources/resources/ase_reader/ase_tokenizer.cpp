#include "resources/ase_reader/ase_tokenizer.h"

#include <cassert>

void ASEToken::clear() {
    data.clear();
    type = ASETokenType::None;
}

void ASETokenizer::tokenization(const std::string& text) {
    ASEToken currentToken;
    for (const auto& symbol : text) {
        if ((symbol == '\0' || symbol == ' ' || symbol == '\t' ||
             symbol == '\n' || symbol == ':') &&
            currentToken.type != ASETokenType::String) {
            if (currentToken.type != ASETokenType::None) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        } else if (symbol == '"') {
            if (currentToken.type == ASETokenType::String) {
                tokens.push_back(currentToken);
                currentToken.clear();
            } else {
                if (currentToken.type != ASETokenType::None) {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
                currentToken.type = ASETokenType::String;
            }
        } else if (symbol == '*' && currentToken.type == ASETokenType::None) {
            if (currentToken.type != ASETokenType::None) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            currentToken.type = ASETokenType::Key;
        } else if (symbol == '{' && currentToken.type != ASETokenType::String) {
            if (currentToken.type != ASETokenType::None) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            currentToken.type = ASETokenType::BlockStart;
            tokens.emplace_back(currentToken);
            currentToken.clear();
        } else if (symbol == '}' && currentToken.type != ASETokenType::String) {
            if (currentToken.type != ASETokenType::None) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            currentToken.type = ASETokenType::BlockEnd;
            tokens.emplace_back(currentToken);
            currentToken.clear();
        } else if (currentToken.type == ASETokenType::None) {
            currentToken.type = ASETokenType::Value;
            currentToken.data += symbol;
        } else {
            currentToken.data += symbol;
        }
    }
    currentTokenIt = std::begin(tokens);
}

ASEToken ASETokenizer::getCurrentTokent() const {
    return *currentTokenIt;
}

ASEToken ASETokenizer::nextToken() {
    if (!isLastToken()) {
        do {
            ++currentTokenIt;
        } while (currentTokenIt != std::end(tokens) &&
                 currentTokenIt->type == ASETokenType::Skip);
    }
    return *currentTokenIt;
}

bool ASETokenizer::isLastToken() const {
    auto nextTokenIt = currentTokenIt;
    do {
        ++nextTokenIt;
    } while (nextTokenIt != std::end(tokens) &&
             nextTokenIt->type == ASETokenType::Skip);
    return nextTokenIt == std::end(tokens);
}

ASEToken ASETokenizer::peekNextToken(size_t offset) const {
    auto nextTokenIt = std::next(currentTokenIt, offset);
    while (nextTokenIt != std::end(tokens) &&
           nextTokenIt->type == ASETokenType::Skip) {
        ++nextTokenIt;
    }
    if (nextTokenIt == std::end(tokens)) {
        return ASEToken();
    }
    return *nextTokenIt;
}

void ASETokenizer::markSkip(size_t offset) {
    auto nextTokenIt = std::next(currentTokenIt, offset);
    while (nextTokenIt != std::end(tokens) &&
           nextTokenIt->type == ASETokenType::Skip) {
        ++nextTokenIt;
    }
    nextTokenIt->type = ASETokenType::Skip;
}