#pragma once
#include "resources/ase_reader/ase_reader_node.h"
#include "resources/ase_reader/ase_tokenizer.h"
#include "resources/ase_reader/rules/rule.h"
#include "resources/ase_reader/rules/int.h"
#include <memory>

template <typename T>
class NumArrayASEReaderNodeRule : public IASEReaderNodeRule {
public:
    std::shared_ptr<ASEReaderNode>
    analyze(ASETokenizer& tokenizer) const override;
};

template <typename T>
std::shared_ptr<ASEReaderNode>
NumArrayASEReaderNodeRule<T>::analyze(ASETokenizer& tokenizer) const {
    auto result = std::make_shared<ASEReaderNode>(ASEReaderNodeType::Array);
    T rule;
    tokenizer.nextToken();
    while (tokenizer.getCurrentTokent().type != ASETokenType::BlockEnd) {
        auto index = std::stoi(tokenizer.peekNextToken().data);
        tokenizer.markSkip();
        if(auto element = rule.analyze(tokenizer)) {
            result->addElement(index, element);
        }
        else {
            tokenizer.nextToken();
            assert(false);
        }
    }
    tokenizer.nextToken();
    return result;
}