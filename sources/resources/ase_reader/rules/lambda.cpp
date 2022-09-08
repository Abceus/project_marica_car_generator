#include "resources/ase_reader/rules/lambda.h"

LambdaASEReaderNodeRule::LambdaASEReaderNodeRule(
    const std::function<
        std::shared_ptr<ASEReaderNode>(ASETokenizer& tokenizer)>& func)
    : func(func) {}

std::shared_ptr<ASEReaderNode> LambdaASEReaderNodeRule::analyze(ASETokenizer& tokenizer) const {
    if (func) {
        return func(tokenizer);
    }
    return nullptr;
}