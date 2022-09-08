#pragma once
#include "resources/ase_reader/ase_reader_node.h"
#include "resources/ase_reader/rules/rule.h"
#include <memory>
#include "resources/ase_reader/ase_tokenizer.h"

class ASEReaderNodeAnalyzer {
public:
    static ASEReaderNodeAnalyzer& Instance();

    std::shared_ptr<ASEReaderNode> analyze(ASETokenizer& tokenizer) const;

    void addRule(const std::string& key,
                 const std::shared_ptr<IASEReaderNodeRule>& rule);

private:
    std::map<std::string, std::shared_ptr<IASEReaderNodeRule>> rules;
};