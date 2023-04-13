#include "ClauseGroup.h"

// ========================
// ClauseGroup
// ========================

int ClauseGroup::getNumClauses() const {
    return this->numClauses;
}


// ========================
// SynonymClauseGroup
// ========================

SynonymClauseGroup::SynonymClauseGroup() = default;

SynonymClauseGroup::SynonymClauseGroup(std::unordered_map<std::string, std::vector<std::shared_ptr<Clause>>>& synonymMap,
                         const std::unordered_set<std::string>& commonSynonyms)
    : synonymMap(std::move(synonymMap)), commonSynonyms(commonSynonyms) {}

void SynonymClauseGroup::addClause(std::shared_ptr<Clause>& c) {
    auto clauseLHS = c->getLHS();
    auto clauseRHS = (c->clauseType == "AssignPatternClause"
                      || c->clauseType == "IfPatternClause"
                      || c->clauseType == "WhilePatternClause")
                              ? c->getAssignSynonym()
                              : c->getRHS();

    // if both are not synonyms, skip
    assert(c->isSynonym(clauseLHS) || c->isSynonym(clauseRHS));
    this->numClauses++;

    // Add LHS synonym, if any
    if (c->isSynonym(clauseLHS)) {
        if (synonymMap.find(clauseLHS) == synonymMap.end()) {
            synonymMap[clauseLHS] = std::vector<std::shared_ptr<Clause>>();
        }
        synonymMap[clauseLHS].push_back(c);
        this->commonSynonyms.insert(clauseLHS);
    }

    // Add RHS synonym, if any
    if (c->isSynonym(clauseRHS)) {
        if (synonymMap.find(clauseRHS) == synonymMap.end()) {
            synonymMap[clauseRHS] = std::vector<std::shared_ptr<Clause>>();
        }
        synonymMap[clauseRHS].push_back(c);
        this->commonSynonyms.insert(clauseRHS);
    }

    this->totalPriorityScore += c->getPriorityScore();
}

ClauseResult SynonymClauseGroup::reduce(QPSFacade& qpsFacade,
                                 const std::unordered_map<std::string, std::string>& synonymTypes) {
    if (synonymMap.empty()) return ClauseResultBuilder::createBooleanResult(true);

    // Initialise priority queue, set of computed synonyms, visited array
    std::priority_queue<ClausePtr, std::vector<ClausePtr>, ClauseComparator> pq;
    std::unordered_set<std::string> computedSynonyms;
    std::unordered_set<std::shared_ptr<Clause>> visited;

    // Take random key from synonymMap, add to computedSynonyms
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, synonymMap.size() - 1);
    auto random_it = std::next(synonymMap.begin(), distrib(gen));
    auto synonymChosen = random_it->first;
    computedSynonyms.insert(synonymChosen);

    // Add all clauses with random key to priority queue
    for (auto& c : synonymMap[synonymChosen]) {
        pq.push(c);
    }

    // Initialise default value
    ClauseResult res = ClauseResultBuilder::createBooleanResult(true);

    // Loop through priority queue
    while (!pq.empty()) {
        auto currClause = pq.top();
        pq.pop();

        // If visited, skip
        if (visited.find(currClause) != visited.end()) continue;
        // Else mark as visited
        visited.insert(currClause);

        // Evaluate ClauseResult
        auto currResult = currClause->evaluate(qpsFacade, synonymTypes);;

        // Early termination if boolean result is false
        if (!currResult.extractQueryBoolean()) return currResult;

        // Add related clauses to pq if synonym has not yet been visited
        for (const auto &synonym : currResult.getSynonymList()) {
            if (computedSynonyms.find(synonym) != computedSynonyms.end()) continue;

            for (auto& c : synonymMap[synonym]) pq.push(c);
            computedSynonyms.insert(synonym);
        }

        // Inner join
        res = ClauseResultEvaluator::evaluate(res, currResult);
    }

    return res;
}

void SynonymClauseGroup::combine(SynonymClauseGroup& other) {
    // Combine synonym maps
    for (const auto& [synonym, clauseList] : other.synonymMap) {
        for (const auto& clause : clauseList) {
            synonymMap[synonym].push_back(clause);
        }
    }

    // Won't have repetitions, since each ClauseGroup has unique synonyms from each other
    this->numClauses += other.numClauses;

    // Combine synonym lists
    this->commonSynonyms.insert(other.commonSynonyms.begin(), other.commonSynonyms.end());
    this->totalPriorityScore += other.totalPriorityScore;
}

std::unordered_set<std::string> SynonymClauseGroup::getCommonSynonyms() const {
    return this->commonSynonyms;
}

int SynonymClauseGroup::getTotalPriorityScore() const {
    return this->totalPriorityScore;
}


// ========================
// NonSynonymClauseGroup
// ========================

void NonSynonymClauseGroup::addClause(std::shared_ptr<Clause> &c) {
    // Assert that clause LHS and RHS are not synonyms
    assert(!c->isSynonym(c->getLHS()) && !c->isSynonym(c->getRHS()));

    this->clauses.push_back(c);
    this->numClauses++;
}

ClauseResult NonSynonymClauseGroup::reduce(QPSFacade &qpsFacade,
                                           const std::unordered_map<std::string, std::string> &synonymTypes) {
    for (const auto& c : this->clauses) {
        auto currResult = c->evaluate(qpsFacade, synonymTypes);
        if (!currResult.extractQueryBoolean()) return currResult;  // Early termination if any evaluates to false
    }

    return ClauseResultBuilder::createBooleanResult(true);
}
