#pragma once
#include "resources/ase_reader/rules/rule.h"
#include <functional>

class LambdaASEReaderNodeRule : public IASEReaderNodeRule {
public:
    LambdaASEReaderNodeRule(const std::function<std::shared_ptr<ASEReaderNode>(
                                ASETokenizer& tokenizer)>& func);

    std::shared_ptr<ASEReaderNode>
    analyze(ASETokenizer& tokenizer) const override;

private:
    std::function<std::shared_ptr<ASEReaderNode>(ASETokenizer& tokenizer)> func;
};