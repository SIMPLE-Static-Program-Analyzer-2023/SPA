//
// Created by Vanessa Khor on 27/3/23.
//

#pragma once
#include <string>
#include <vector>
#include <tuple>

class QueryTarget {
private:
    std::string type;  // bool, syn, tuple
    std::vector<std::string> multipleTargets;
    std::string synTarget;

public:
    QueryTarget();
    void setSyn(std::string selected);
    void setTuple(std::vector<std::string> selected);
    void setBool();
    std::vector<std::string> getTarget();
};
