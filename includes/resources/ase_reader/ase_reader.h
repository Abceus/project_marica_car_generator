#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "resources/ase_reader/ase_reader_node.h"

class ASEReader {
public:
    void init(const std::filesystem::path& path);
    std::weak_ptr<ASEReaderNode> getRootNode() const;

private:
    std::shared_ptr<ASEReaderNode> rootNode;
};