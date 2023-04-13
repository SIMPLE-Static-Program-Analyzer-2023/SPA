#pragma once

#include <memory>
#include <vector>
#include <string>

class Visitor;

class TNode;
class NumberedNode;

using Node = std::shared_ptr<TNode>;

class TNode {
protected:
    Node parent;
public:
    explicit TNode(Node parent);
    virtual ~TNode() = default;
    virtual bool operator==(const TNode& other) const = 0;
    virtual std::string toString() = 0;
    virtual void accept(Visitor *v) = 0;
    Node getParent() const;
    void setParent(Node parent);
};

// abstract class for nodes with statement number
class NumberedNode : public TNode {
protected:
    int stmtNo;
public:
    explicit NumberedNode(Node parent, int stmtNo);
    int getStmtNo() const;
    virtual std::string toString() = 0;
    virtual void accept(Visitor *v) = 0;
};
