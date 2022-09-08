#pragma once

#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

using ASEReaderVariant = std::variant<float, int, std::string>;

enum class ASEReaderNodeType { None, Value, Dict, Array };

class ASEReaderNode {
public:
    ASEReaderNode(ASEReaderNodeType type = ASEReaderNodeType::None);

    template <typename T>
    void setValue(const T& value);

    template <typename T>
    T getValue() const;

    std::shared_ptr<ASEReaderNode> addChild(const std::string& key,
                                            ASEReaderNodeType type = ASEReaderNodeType::None);
    std::shared_ptr<ASEReaderNode> addChild(const std::string& key,
                                            const std::shared_ptr<ASEReaderNode>& child);
    std::shared_ptr<ASEReaderNode> getChild(const std::string& key) const;
    bool hasChild(const std::string& key) const;

    std::shared_ptr<ASEReaderNode> addElement(ASEReaderNodeType type = ASEReaderNodeType::None);
    std::shared_ptr<ASEReaderNode> addElement(const std::shared_ptr<ASEReaderNode>& element);
    std::shared_ptr<ASEReaderNode> addElement(size_t index, const std::shared_ptr<ASEReaderNode>& element);
    std::shared_ptr<ASEReaderNode> getElement(size_t index) const;
    size_t getSize() const;

    ASEReaderNodeType getType() const;

private:
    ASEReaderVariant value;
    std::map<std::string, std::shared_ptr<ASEReaderNode>> children;
    std::vector<std::shared_ptr<ASEReaderNode>> elements;
    ASEReaderNodeType type = ASEReaderNodeType::None;
};

template <typename T>
void ASEReaderNode::setValue(const T& v) {
    value = v;
}

template <typename T>
T ASEReaderNode::getValue() const {
    return std::get<T>(value);
}
