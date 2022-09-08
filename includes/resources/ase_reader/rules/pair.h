#pragma once
#include "resources/ase_reader/ase_reader_node.h"
#include "resources/ase_reader/rules/rule.h"
#include <memory>

template <typename T1, typename T2>
class PairASEReaderNodeRule : public IASEReaderNodeRule {
public:
    std::shared_ptr<ASEReaderNode>
    analyze(ASETokenizer& tokenizer) const override;
};

template <typename T1, typename T2>
std::shared_ptr<ASEReaderNode>
PairASEReaderNodeRule<T1, T2>::analyze(ASETokenizer& tokenizer) const {
    auto result = std::make_shared<ASEReaderNode>(ASEReaderNodeType::Array);
    T1 rule1;
    result->addElement(rule1.analyze(tokenizer));
    T2 rule2;
    result->addElement(rule2.analyze(tokenizer));
    return result;
}