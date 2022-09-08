#include "resources/ase_reader/rules/value.h"
#include "resources/ase_reader/ase_reader_node_analyzer.h"
#include <memory>

std::shared_ptr<ASEReaderNode>
ValueASEReaderNodeRule::analyze(ASETokenizer& tokenizer) const {
    auto result = std::make_shared<ASEReaderNode>(ASEReaderNodeType::Dict);
    auto token = tokenizer.getCurrentTokent();
    result->addChild(token.data, ASEReaderNodeAnalyzer::Instance().analyze(tokenizer));
    return result;
}