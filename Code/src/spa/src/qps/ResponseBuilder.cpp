//
// Created by Vanessa Khor on 5/2/23.
//
#include "ResponseBuilder.h"

ResponseBuilder::ResponseBuilder(QPSFacade& qpsf, Query& query) : qpsf(qpsf), query(query) {}

std::vector<std::string> ResponseBuilder::formatOutput(ClauseResult &res) {
    std::vector<std::string> targetLst = query.getTargetLst();
    std::vector<std::string> output;
    // std::string formattedOutput = this->formatString(res.extractQueryData(this->qpsf, name, type));
    if (targetLst[0] == "_BOOLEAN") {
        output.push_back(res.extractQueryBoolean() ? "TRUE" : "FALSE");
    }
    else if (targetLst.size() == 1) {
        std::string synonym = targetLst[0];
        std::string type;
        if (targetLst[0].find('.') < targetLst[0].length()) {
            synonym = getAttributeSynonym(targetLst[0]);
            type = this->findTypeOf(synonym);
            std::string attribute = targetLst[0].substr(targetLst[0].find(".") + 1);
            type = type + "." + attribute;
        }
        else {
            type = this->findTypeOf(synonym);
        }
        output = res.extractQueryData(this->qpsf, synonym, type);
    }
    else {
        std::unordered_map<std::string, std::string> synonymTypes;
        for (auto declaration : query.getDeclarationList()) {
            for (const auto& synonym : declaration.getSynonymList()) {
                synonymTypes[synonym] = declaration.getType();
            }
        }
        std::vector<std::string> synonymList;
        for (auto target : targetLst) {
            if (target.find('.') < target.length()) {
                std::string synonym = getAttributeSynonym(target);
                std::string type = this->findTypeOf(synonym);
                std::string attribute = target.substr(target.find(".") + 1);
                type = type + "." + attribute;
                synonymTypes[synonym] = type;
                synonymList.push_back(synonym);
            }
            else {
                synonymList.push_back(target);
            }
        }

        std::vector<std::vector<std::string>> tuple = res.extractQueryTuple(synonymList,
                                                                            synonymTypes,
                                                                            this->qpsf);
        for (auto row : tuple) {
            std::string rowStr = "";
            for (auto colVal : row) {
                rowStr += colVal + " ";
            }
            rowStr.pop_back();
            output.push_back(rowStr);
        }
    }
    return output;
}


std::string ResponseBuilder::findTypeOf(const std::string& name) {
    for (auto declaration : query.getDeclarationList()) {
        for (const auto& synonym : declaration.getSynonymList()) {
            if (synonym == name) {
                return declaration.getType();
            }
        }
    }

    throw QpsSyntaxException("Synonym " + name + " is not included in any declarations");
}

/*
std::string ResponseBuilder::formatString(std::vector<std::string> list) {
    std::string str = list[0];
    for (auto& i : list) {
        if (i == list[0]) {
            continue;
        }
        str += ", " + i;
    }
    return str;
}
*/

std::string ResponseBuilder::getAttributeSynonym(const std::string& s) {
    std::string::size_type pos = s.find('.');
    if (pos != std::string::npos) {
        return s.substr(0, pos);
    }
    else {
        return s;
    }
}
