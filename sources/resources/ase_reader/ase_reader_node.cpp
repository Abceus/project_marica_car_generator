#include "resources/ase_reader/ase_reader_node.h"
#include "resources/ase_reader/rules/array.h"
#include <memory>

ASEReaderNode::ASEReaderNode(ASEReaderNodeType type) : type(type) {}

std::shared_ptr<ASEReaderNode> ASEReaderNode::addChild(const std::string& key,
                                                       ASEReaderNodeType type) {
    auto found = children.find(key);
    if(found == std::end(children)) {
        children.insert({key, std::make_shared<ASEReaderNode>(ASEReaderNodeType::Array)});
        found = children.find(key);
    }
    return found->second->addElement(std::make_shared<ASEReaderNode>(type));
}

std::shared_ptr<ASEReaderNode>
ASEReaderNode::addChild(const std::string& key,
                        const std::shared_ptr<ASEReaderNode>& child) {
    auto found = children.find(key);
    if(found == std::end(children)) {
        children.insert({key, std::make_shared<ASEReaderNode>(ASEReaderNodeType::Array)});
        found = children.find(key);
    }
    return found->second->addElement(child);
}
std::shared_ptr<ASEReaderNode>
ASEReaderNode::getChild(const std::string& key) const {
    auto found = children.find(key);
    if (found != std::end(children)) {
        if(found->second->getSize() > 1) {
            return found->second;
        }
        return found->second->getElement(0);
    }
    return nullptr;
}

bool ASEReaderNode::hasChild(const std::string& key) const {
    return children.find(key) != std::end(children);
}

std::shared_ptr<ASEReaderNode>
ASEReaderNode::addElement(ASEReaderNodeType type) {
    elements.emplace_back(std::make_shared<ASEReaderNode>(type));
    return elements.back();
}

std::shared_ptr<ASEReaderNode>
ASEReaderNode::addElement(const std::shared_ptr<ASEReaderNode>& element) {
    elements.emplace_back(element);
    return elements.back();
}

std::shared_ptr<ASEReaderNode>
ASEReaderNode::addElement(size_t index,
                          const std::shared_ptr<ASEReaderNode>& element) {
    if (index >= elements.size()) {
        elements.resize(index + 1);
    }
    elements[index] = element;
    return element;
}

std::shared_ptr<ASEReaderNode> ASEReaderNode::getElement(size_t index) const {
    return elements.at(index);
}

size_t ASEReaderNode::getSize() const {
    return elements.size();
}

ASEReaderNodeType ASEReaderNode::getType() const {
    return type;
}
