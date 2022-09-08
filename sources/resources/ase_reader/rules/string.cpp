#include "resources/ase_reader/rules/string.h"

std::shared_ptr<ASEReaderNode> StringASEReaderNodeRule::analyze(ASETokenizer& tokenizer) const {
    auto result = std::make_shared<ASEReaderNode>(ASEReaderNodeType::Value);
    result->setValue(tokenizer.getCurrentTokent().data);
    tokenizer.nextToken();
    return result;
}