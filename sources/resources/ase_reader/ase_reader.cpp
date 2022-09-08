#include "resources/ase_reader/ase_reader.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <regex>

#include "resources/ase_reader/ase_reader_node.h"
#include "resources/ase_reader/ase_reader_node_analyzer.h"
#include "resources/ase_reader/ase_tokenizer.h"
#include "resources/ase_reader/rules/dict.h"

void ASEReader::init(const std::string& path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    auto fileString = buffer.str();
    ASETokenizer tokenizer;
    tokenizer.tokenization(fileString);
    DictASEReaderNodeRule rule;
    rootNode = rule.analyze(tokenizer);
}

std::weak_ptr<ASEReaderNode> ASEReader::getRootNode() const {
    return rootNode;
}