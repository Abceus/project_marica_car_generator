#pragma once
#include "resources/ase_reader/ase_reader_node.h"
#include "resources/ase_reader/ase_tokenizer.h"
#include "resources/ase_reader/rules/rule.h"
#include <memory>
#include <cassert>

template <typename T, size_t size>
class ArrayASEReaderNodeRule : public IASEReaderNodeRule {
public:
    std::shared_ptr<ASEReaderNode>
    analyze(ASETokenizer& tokenizer) const override;
};

template <typename T, size_t size>
std::shared_ptr<ASEReaderNode>
ArrayASEReaderNodeRule<T, size>::analyze(ASETokenizer& tokenizer) const {
    auto result = std::make_shared<ASEReaderNode>(ASEReaderNodeType::Array);
    T rule;
    for(size_t i = 0; i < size; ++i) {
        if(auto element = rule.analyze(tokenizer)) {
            result->addElement(element);
        }
        else {
            tokenizer.nextToken();
            assert(false);
        }
    }
    return result;
}