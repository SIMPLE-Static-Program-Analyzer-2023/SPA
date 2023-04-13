//
// Created by Vanessa Khor on 5/2/23.
//
#include "Declaration.h"

Declaration::Declaration(std::string type, std::vector<std::string> synonyms) : type(type), synonymList(synonyms) {};

void Declaration::printDeclaration() {
    std::cout << type + ": ";
    for (std::string s : synonymList) {
        std::cout << s + ' ';
    }
    std::cout << std::endl;
}

std::string Declaration::getType() {
    return type;
}

std::vector<std::string> Declaration::getSynonymList() {
    return synonymList;
}

class variable : public Declaration {
public:
};

class assign : public Declaration {
public:
};

class _while : public Declaration {
public:
};

class procedure : public Declaration {
public:
};
