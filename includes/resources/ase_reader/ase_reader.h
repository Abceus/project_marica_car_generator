#pragma once

#include <memory>
#include <string>

#include "resources/ase_reader/ase_reader_node.h"

class ASEReader {
public:
    void init(const std::string& path);
    std::weak_ptr<ASEReaderNode> getRootNode() const;

private:
    std::shared_ptr<ASEReaderNode> rootNode;
};