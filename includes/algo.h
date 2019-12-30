#pragma once
#include <vector>
#include <string>
#include <set>

struct Rule {
    char left;
    std::string right;

    Rule(char left, std::string& right) : left(left), right(right) {}

    friend bool operator<(const Rule& lhs, const Rule& rhs);
};

class Grammar {
public:
    std::set<char> terminals;
    std::set<char> nonterminals;
    char S;
    char S1;
    std::set<Rule> rules;

    Grammar(std::set<char>& terminals, std::set<char>& nonterminals, char S, std::set<Rule>& rules) :
            terminals(terminals), nonterminals(nonterminals), S(S), S1('\0'), rules(rules) {}
};

struct Situation {
    char left;
    std::string right;
    int dot_pos;
    int pos;

    Situation(char _left, char _right, int _dot_pos, int _pos);
    Situation(const Rule& _rule, int _pos);
    Situation shift();

    friend bool operator<(const Situation& lhs, const Situation& rhs);
};

struct Configuration {
    std::vector<std::set<Situation>> situations;
};

class Algo {
public:
    explicit Algo(Grammar& g) : gram(g) {}
    bool predict(const std::string& word);

    std::set<Situation> Scan(int pos);
    std::set<Situation> Predict(int pos);
    std::set<Situation> Complete(int pos);
    void build_configuration(const std::string& word);
private:
    Configuration conf;
    Grammar gram;
    std::string current_word;
};