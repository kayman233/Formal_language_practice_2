#include "algo.h"

Situation::Situation(char _left, char _right, int _dot_pos, int _pos) {
    left = _left;
    right = _right;
    dot_pos = _dot_pos;
    pos = _pos;
}

Situation::Situation(const Rule& _rule, int _pos) {
    left = _rule.left;
    right = _rule.right;
    dot_pos = 0;
    pos = _pos;
}

bool operator<(const Rule& lhs, const Rule& rhs) {
    if (lhs.left < rhs.left) {
        return true;
    }
    else if (lhs.left == rhs.left) {
        if (lhs.right < rhs.right) {
            return true;
        }
    }
    return false;
}

bool operator<(const Situation& lhs, const Situation& rhs) {
    if (lhs.left < rhs.left) {
        return true;
    }
    else if (lhs.left == rhs.left) {
        if (lhs.right < rhs.right) {
            return true;
        }
        else if (lhs.right == rhs.right) {
            if (lhs.dot_pos < rhs.dot_pos) {
                return true;
            }
            else if (lhs.dot_pos == rhs.dot_pos) {
                return lhs.pos < rhs.pos;
            }
        }
    }

    return false;
}

Situation Situation::shift() { // move dot
    Situation new_sit(*this);
    ++new_sit.dot_pos;

    return new_sit;
}

bool Algo::predict(const std::string& word) {

    build_configuration(word);
    for (Situation sit : conf.situations[word.length()]) {
        if (sit.left == gram.S1 && sit.right[0] == gram.S
            && sit.dot_pos == sit.right.length() && sit.pos == 0) {
            return true;
        }
    }

    return false;
}

void Algo::build_configuration(const std::string& word) {
    current_word = word;
    Situation initial_sit(gram.S1, gram.S, 0, 0); //convert
    std::set<Situation> situations;
    situations.insert(initial_sit);
    conf.situations.push_back(situations);

    for (int i = 1; i <= current_word.length(); ++i) {
        std::set<Situation> null_situations; //null sit
        conf.situations.push_back(null_situations);
    }

    for (int j = 0; j <= current_word.length(); ++j) {
        Scan(j);
        bool has_changes = true;
        int prev = conf.situations[j].size();
        while (has_changes) { // changes
            Complete(j);
            Predict(j);
            if (prev == conf.situations[j].size()) {
                has_changes = false;
            }
            prev = conf.situations[j].size();
        }
    }
}

std::set<Situation> Algo::Scan(int pos) {
    std::set<Situation> added;
    if (pos == 0) {
        return added;
    }

    for (Situation sit : conf.situations[pos - 1]) {
        if (sit.right[sit.dot_pos] == current_word[pos - 1]) {
            conf.situations[pos].insert(sit.shift());
            added.insert(sit.shift());
        }
    }

    return added;
}

std::set<Situation> Algo::Predict(int pos) {
    std::set<Situation> added;
    for (Situation sit : conf.situations[pos]) {
        char non_term = sit.right[sit.dot_pos]; // check non-term and in complete
        if (gram.nonterminals.find(non_term) == gram.nonterminals.end()) {
            continue;
        }
        for (const Rule& rule : gram.rules) {
            if (rule.left == non_term) {
                Situation new_sit(rule, pos); // get from rule
                conf.situations[pos].insert(new_sit);
                added.insert(new_sit);
            }
        }
    }

    return added;
}

std::set<Situation> Algo::Complete(int pos) {
    std::set<Situation> added;
    for (const Situation& sit : conf.situations[pos]) {
        if (sit.dot_pos == sit.right.length()) { //. in the end
            for (Situation sit_1 : conf.situations[sit.pos]) {
                char non_term = sit_1.right[sit_1.dot_pos]; // check non-term
                if (gram.nonterminals.find(non_term) == gram.nonterminals.end()) {
                    continue;
                }
                conf.situations[pos].insert(sit_1.shift());
                added.insert(sit_1.shift());
            }
        }
    }

    return added;
}