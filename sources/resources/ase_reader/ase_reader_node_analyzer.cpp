#include "resources/ase_reader/ase_reader_node_analyzer.h"
#include "resources/ase_reader/ase_tokenizer.h"
#include <memory>

ASEReaderNodeAnalyzer& ASEReaderNodeAnalyzer::Instance() {
    static ASEReaderNodeAnalyzer single;
    return single;
}

std::shared_ptr<ASEReaderNode>
ASEReaderNodeAnalyzer::analyze(ASETokenizer& tokenizer) const {
    auto token = tokenizer.getCurrentTokent();
    tokenizer.nextToken();
    if (token.type == ASETokenType::Key) {
        auto command = token.data;
        auto found = rules.find(command);
        if (found != std::end(rules)) {
            return found->second->analyze(tokenizer);
        } else {
            int blockCount = 0;
            while (!tokenizer.isLastToken()) {
                if(tokenizer.getCurrentTokent().type == ASETokenType::Key && blockCount <= 0) {
                    break;
                }
                if(tokenizer.getCurrentTokent().type == ASETokenType::BlockStart) {
                    blockCount++;
                }
                if(tokenizer.getCurrentTokent().type == ASETokenType::BlockEnd) {
                    blockCount--;
                }
                if(blockCount < 0) {
                    break;
                }
                tokenizer.nextToken();
            };
        }
    }
    return nullptr;
}

void ASEReaderNodeAnalyzer::addRule(
    const std::string& key, const std::shared_ptr<IASEReaderNodeRule>& rule) {
    rules.emplace(key, rule);
}