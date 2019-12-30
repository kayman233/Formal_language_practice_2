#include <gtest/gtest.h>
#include <algo.h>
#include <stdexcept>
#include <vector>

TEST(rule_test, test0) {
    char c;
    std::string s;
    c = 'S';
    s = "aS";

    ASSERT_NO_THROW(Rule(c,s));
}

TEST(rule_test, test1) {
    char c;
    std::string s;
    c = 'S';
    s = "aS";
    Rule first(c,s);
    c = 'S';
    s = "bS";
    Rule second(c, s);
    ASSERT_LT(first, second);
}

TEST(grammar_test, test0) {
    std::set<char> term;
    term.insert('a');
    term.insert('b');
    term.insert('c');
    std::set<char> non_term;
    non_term.insert('S');
    non_term.insert('T');
    std::set<Rule> rules;
    char c;
    std::string s;
    c = 'S';
    s = "aS";
    Rule first(c,s);
    rules.insert(first);
    c = 'S';
    s = "bS";
    Rule second(c, s);
    rules.insert(second);
    Grammar gram(term, non_term,'S',rules);
    ASSERT_EQ(gram.rules.size(), 2);
}

TEST(situation_test, test0) {

    char c;
    char s;
    c = 'S';
    s = 'a';
    Situation first(c, s,0,0);
    ASSERT_EQ(first.right, "a");
    ASSERT_EQ(first.dot_pos, 0);
}
TEST(situation_test, test1) {

    char c;
    std::string s;
    c = 'S';
    s = "aS";
    Rule first(c,s);
    Situation sit(first, 1);
    ASSERT_EQ(sit.right, s);
    ASSERT_EQ(sit.dot_pos, 0);
}

TEST(situation_test, test2) {

    char c;
    std::string s;
    c = 'S';
    s = "abS";
    Rule first(c,s);
    Situation sit(first, 1);
    Situation sit_shifted = sit.shift();
    ASSERT_EQ(sit_shifted.dot_pos, 1);
}

class TestAnalyser : public ::testing::Test {
protected:
    void SetUp() {
        std::set<char> term;
        term.insert('a');
        term.insert('b');
        std::set<char> non_term;
        non_term.insert('S');
        std::set<Rule> rules;
        char c;
        std::string s;
        c = 'S';
        s = "aS";
        Rule first(c,s);
        rules.insert(first);
        c = 'S';
        s = "b";
        Rule second(c, s);
        rules.insert(second);
        Grammar gram(term, non_term,'S',rules);
        analyzer = new Algo(gram);
    }
    void TearDown() {
        delete analyzer;
    }
    Algo* analyzer;
};

TEST_F(TestAnalyser, scan_test_0) {
    ASSERT_EQ(analyzer->Scan(0).size(), 0);
}

TEST_F(TestAnalyser, complete_test_0) {
    analyzer->build_configuration("b");
    std::set<Situation> ans = analyzer->Complete(0);
    ASSERT_EQ(ans.size(), 0);
}

TEST_F(TestAnalyser, predict_test_0) {
    analyzer->build_configuration("b");
    std::set<Situation> ans = analyzer->Predict(0);
    char c;
    std::string s;
    c = 'S';
    s = "aS";
    Rule rule(c, s);
    Situation sit(rule, 0);
    ASSERT_NE(ans.find(sit), ans.end());
}

TEST_F(TestAnalyser, predict_test_1) {
    analyzer->build_configuration("b");
    std::set<Situation> ans = analyzer->Predict(0);
    char c;
    std::string s;
    c = 'S';
    s = "b";
    Rule rule1(c, s);
    Situation sit1(rule1, 0);
    ASSERT_NE(ans.find(sit1), ans.end());
}

TEST_F(TestAnalyser, scan_test_1) {
    analyzer->build_configuration("b");
    analyzer->Scan(0);
    analyzer->Complete(0);

    std::set<Situation> ans = analyzer->Scan(1);
    char c;
    std::string s;
    c = 'S';
    s = "b";
    Rule rule1(c, s);
    Situation sit1(rule1, 0);
    sit1 = sit1.shift();
    ASSERT_NE(ans.find(sit1), ans.end());
}

TEST_F(TestAnalyser, complete_test_1) {
    analyzer->build_configuration("b");
    analyzer->Scan(0);
    analyzer->Complete(0);
    analyzer->Scan(1);
    std::set<Situation> ans = analyzer->Complete(1);
    char c;
    std::string s;
    c = 'S';
    s = "aS";
    Rule rule1(c, s);
    Situation sit1(rule1, 0);
    sit1 = sit1.shift();
    sit1 = sit1.shift();
    ASSERT_EQ(ans.find(sit1), ans.end());
}

TEST_F(TestAnalyser, usual_test_0) {
    ASSERT_EQ(analyzer->predict("a"), false);
}

TEST_F(TestAnalyser, usual_test_1) {
    ASSERT_EQ(analyzer->predict("b"), true);
}

TEST_F(TestAnalyser, usual_test_3) {
    ASSERT_EQ(analyzer->predict("aaaaab"), true);
}

class TestAnalyser_2 : public ::testing::Test {
protected:
    void SetUp() {
        std::set<char> term;
        term.insert('+');
        term.insert('*');
        term.insert('(');
        term.insert(')');
        std::set<char> non_term;
        non_term.insert('S');
        non_term.insert('T');
        non_term.insert('F');
        std::set<Rule> rules;
        char c;
        std::string s;
        c = 'S';
        s = "T+S";
        Rule first(c,s);
        rules.insert(first);
        c = 'S';
        s = "T";
        Rule second(c, s);
        rules.insert(second);
        c = 'T';
        s = "F*T";
        Rule third(c, s);
        rules.insert(third);
        c = 'F';
        s = "(S)";
        Rule fourth(c, s);
        rules.insert(fourth);
        c = 'F';
        s = "a";
        Rule fifth(c, s);
        rules.insert(fifth);
        Grammar gram(term, non_term,'S',rules);
        analyzer = new Algo(gram);
    }
    void TearDown() {
        delete analyzer;
    }
    Algo* analyzer;
};

TEST_F(TestAnalyser_2, usual_test_0) {
    ASSERT_EQ(analyzer->predict("(a+a)"), true);
}

TEST_F(TestAnalyser_2, usual_test_1) {
    ASSERT_EQ(analyzer->predict("a+a+(a*a)"), false);
}

TEST_F(TestAnalyser_2, usual_test_3) {
    ASSERT_EQ(analyzer->predict("(a+(a*a))*(a+a)"), true);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}