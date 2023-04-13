//
// Created by Vanessa Khor on 27/3/23.
//
#include "QueryTarget.h"

QueryTarget::QueryTarget() {}

void QueryTarget::setSyn(std::string selected) {
    synTarget = selected;
    type = "syn";
}

void QueryTarget::setTuple(std::vector<std::string> selected) {
    multipleTargets = selected;
    type = "tuple";
}

void QueryTarget::setBool() {
    type = "bool";
}

std::vector<std::string> QueryTarget::getTarget() {
    std::vector<std::string> _target;
    if (type == "bool") {
        _target.push_back("_BOOLEAN");
    } else if (type == "syn") {
        _target.push_back(synTarget);
    } else {
        return multipleTargets;
    }
    return _target;
}
