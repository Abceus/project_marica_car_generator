#include "resources/ase_reader/rules/float.h"
#include <memory>

std::shared_ptr<ASEReaderNode>
FloatASEReaderNodeRule::analyze(ASETokenizer& tokenizer) const {
    auto result = std::make_shared<ASEReaderNode>(ASEReaderNodeType::Value);
    result->setValue(std::stof(tokenizer.getCurrentTokent().data));
    tokenizer.nextToken();
    return result;
}