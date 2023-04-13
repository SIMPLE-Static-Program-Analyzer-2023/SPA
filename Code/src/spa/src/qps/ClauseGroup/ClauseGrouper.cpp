#include "ClauseGrouper.h"

void ClauseGrouper::addClause(const std::shared_ptr<Clause>& clause) {
    // Check if assign, if, or while pattern clause
    if (clause->clauseType == "AssignPatternClause"
        || clause->clauseType == "IfPatternClause"
        || clause->clauseType == "WhilePatternClause") {
        // Synonyms are definitely assignSynonym and possibly LHS
        const auto LHS = clause->getLHS();
        const auto LHSInfo = extractSynonymInfo(LHS, clause);
        auto assignSynonym = clause->getAssignSynonym();
        auto assignSynonymInfo = extractSynonymInfo(assignSynonym, clause);
        addClause(clause, LHS, LHSInfo, assignSynonym, assignSynonymInfo);
    } else {
        // Synonyms are any, either or none of LHS and RHS
        const auto LHS = clause->getLHS();
        const auto LHSInfo = extractSynonymInfo(LHS, clause);
        const auto RHS = clause->getRHS();
        const auto RHSInfo = extractSynonymInfo(RHS, clause);
        addClause(clause, LHS, LHSInfo, RHS, RHSInfo);
    }
}
void ClauseGrouper::addClause(std::shared_ptr<Clause> clause,
                              const std::string& LHS, SynonymInfo LHSInfo,
                              const std::string& RHS, SynonymInfo RHSInfo) {
    // Check LHS synonym exists in synonymToClauseGroupMap, -1 otherwise
    const auto LHSIndex = LHSInfo.synonymIndex;
    const auto LHSIsSynonym = LHSInfo.isSynonym;

    // Check RHS synonym exists in synonymToClauseGroupMap, -1 otherwise
    const auto RHSIndex = RHSInfo.synonymIndex;
    const auto RHSIsSynonym = RHSInfo.isSynonym;

    if (!RHSIsSynonym && !LHSIsSynonym) {
        // Case 0: Neither are synonyms
        this->nonSynonymGroup.addClause(clause);

    } else if (RHSIsSynonym && LHSIsSynonym
               && RHSIndex != -1 && LHSIndex != -1
               && RHSIndex != LHSIndex) {
        // Case 1: Both synonyms belong to different valid ClauseGroups
        int lengthRHSGroup = this->clauseGroups[RHSIndex].getNumClauses();
        int lengthLHSGroup = this->clauseGroups[LHSIndex].getNumClauses();

        // Longer group takes in the shorter group to combine,
        // replace shorter group with empty group to save memory
        if (lengthRHSGroup >= lengthLHSGroup) {
            this->combineClauseGroups(RHSIndex, LHSIndex);
            this->clauseGroups[RHSIndex].addClause(clause);
        } else {
            this->combineClauseGroups(LHSIndex, RHSIndex);
            this->clauseGroups[LHSIndex].addClause(clause);
        }

    } else if (RHSIsSynonym && RHSIndex != -1) {
        // Case 2: RHS synonym belongs to a ClauseGroup
        // LHS may belong to the same ClauseGroup or none at all, or may not be a synonym
        this->clauseGroups[RHSIndex].addClause(clause);
        if (LHSIsSynonym) this->synonymMap[LHS] = RHSIndex;

    } else if (LHSIsSynonym && LHSIndex != -1) {
        // Case 3: LHS synonym belongs to a ClauseGroup
        // RHS may belong to the same ClauseGroup or none at all, or may not be a synonym
        this->clauseGroups[LHSIndex].addClause(clause);
        if (RHSIsSynonym) this->synonymMap[RHS] = LHSIndex;

    } else {
        // Case 4: Neither belong to a ClauseGroup
        auto newClauseGroup = SynonymClauseGroup();
        newClauseGroup.addClause(clause);
        this->clauseGroups.push_back(newClauseGroup);
        updateClauseGroupMapIndex(newClauseGroup, clauseGroups.size() - 1);
    }
}

void ClauseGrouper::addClauses(Query &query) {
    auto clauses = query.getClauseList();
    for (auto& clause : clauses) this->addClause(clause);
}

void ClauseGrouper::updateClauseGroupMapIndex(SynonymClauseGroup& c, int index) {
    auto synonyms = c.getCommonSynonyms();
    for (auto& synonym : synonyms) {
        this->synonymMap[synonym] = index;
    }
}

SynonymInfo ClauseGrouper::extractSynonymInfo(const std::string& synonym,
                                              const std::shared_ptr<Clause>& c) {
    auto isSynonym = c->isSynonym(synonym);
    int index = isSynonym && this->synonymMap.find(synonym) != this->synonymMap.end()
                   ? this->synonymMap[synonym]
                   : -1;
    return SynonymInfo{ isSynonym, index };
}

void ClauseGrouper::combineClauseGroups(int indexToCombineInto, int indexToCombineFrom) {
    this->clauseGroups[indexToCombineInto].combine(this->clauseGroups[indexToCombineFrom]);
    updateClauseGroupMapIndex(this->clauseGroups[indexToCombineFrom], indexToCombineInto);
    this->clauseGroups[indexToCombineFrom] = SynonymClauseGroup();
}

std::vector<SynonymClauseGroup> ClauseGrouper::getClauseGroups() {
    return this->clauseGroups;
}

NonSynonymClauseGroup ClauseGrouper::getNonSynonymGroup() {
    return this->nonSynonymGroup;
}
