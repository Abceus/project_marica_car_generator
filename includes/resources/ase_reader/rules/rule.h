#pragma once
#include "resources/ase_reader/ase_reader_node.h"
#include "resources/ase_reader/ase_tokenizer.h"

class IASEReaderNodeRule {
public:
    virtual std::shared_ptr<ASEReaderNode>
    analyze(ASETokenizer& tokenizer) const = 0;
};