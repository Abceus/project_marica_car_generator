#pragma once
#include "resources/ase_reader/rules/rule.h"
#include <set>

class SetASEReaderNodeRule : public IASEReaderNodeRule {
public:
    std::shared_ptr<ASEReaderNode>
    analyze(ASETokenizer& tokenizer) const override;
};