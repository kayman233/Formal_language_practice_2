#include <iostream>
#include <string>
#include <set>
#include "algo.h"

int main() {

    int term_size;
    int non_size;
    int rules_size;

    std::cin >> term_size;
    std::cin >> non_size;
    std::cin >> rules_size;

    std::set<char> term;
    std::set<char> non_term;
    std::set<Rule> rules;

    for (int i = 0; i < term_size; ++i) {
        char sym;
        std::cin >> sym;
        term.insert(sym);
    }

    for (int i = 0; i < non_size; ++i) {
        char sym;
        std::cin >> sym;
        non_term.insert(sym);
    }

    char S;
    std::cin >> S;

    for (int i = 0; i < rules_size; ++i) {
        char sym;
        std::cin >> sym;
        std::string right;
        std::cin >> right;
        Rule rule(sym, right);
        rules.insert(rule);
    }

    Grammar gram(term, non_term, S, rules);

    std::string word;
    std::cin >> word;

    Algo earley(gram);
    if (earley.predict(word)) {
        std::cout << "true" << std::endl;
    } else {
        std::cout << "false" << std::endl;
    }

    return 0;
}