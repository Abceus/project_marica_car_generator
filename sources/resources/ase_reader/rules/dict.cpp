#include "resources/ase_reader/rules/dict.h"
#include "resources/ase_reader/ase_reader_node_analyzer.h"
#include "resources/ase_reader/ase_tokenizer.h"
#include <memory>


std::shared_ptr<ASEReaderNode>
DictASEReaderNodeRule::analyze(ASETokenizer& tokenizer) const {
    auto result = std::make_shared<ASEReaderNode>(ASEReaderNodeType::Dict);
    auto token = tokenizer.getCurrentTokent();
    while (token.type != ASETokenType::BlockEnd && !tokenizer.isLastToken()) {
        if (token.type == ASETokenType::Key) {
            if(auto k = ASEReaderNodeAnalyzer::Instance().analyze(tokenizer)) {
                result->addChild(token.data, k);
            }
        } else {
            tokenizer.nextToken();
        }
        token = tokenizer.getCurrentTokent();
    }
    tokenizer.nextToken();
    return result;
}
