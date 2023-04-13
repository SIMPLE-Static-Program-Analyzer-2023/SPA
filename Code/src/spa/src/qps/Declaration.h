//
// Created by Vanessa Khor on 5/2/23.
//
#pragma once
#include <string>
// #include <list>
#include <vector>
#include <iostream>

class Declaration {
private:
    std::string type;
    std::vector<std::string> synonymList;

public:
    explicit Declaration(std::string type, std::vector<std::string> synonyms);

    std::string getType();

    void printDeclaration();

    std::vector<std::string> getSynonymList();

    bool operator==(const Declaration& other) const {
        return type == other.type && synonymList == other.synonymList;
    }
};
