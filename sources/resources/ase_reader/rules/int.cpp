#include "resources/ase_reader/rules/int.h"

std::shared_ptr<ASEReaderNode> IntASEReaderNodeRule::analyze(ASETokenizer& tokenizer) const {
    auto result = std::make_shared<ASEReaderNode>(ASEReaderNodeType::Value);
    result->setValue(std::stoi(tokenizer.getCurrentTokent().data));
    tokenizer.nextToken();
    return result;
}