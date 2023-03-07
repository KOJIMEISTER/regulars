#pragma once
#include <string>
#include <list>
using namespace std;

#define AVAILABLE_UNIT_TEST

struct UnitTest
{
	string formula = "";
	list<string> seq;
	int depth = 0;
};

list <UnitTest> unitTests()
{
	list<UnitTest> tests;
	tests.push_back({ "b*a*b*", {"eps", "b", "a", "ab", "bb", "ba", "bab"}, 1 });
	tests.push_back({ "(a^+)*", {"eps", "a", "aa", "aaa", "aaaa"}, 2 });
	tests.push_back({ "(a+aa)(b+bb)", {"ab", "abb", "aab", "aabb"}, 0 });
	tests.push_back({ "(a+b+(c+d(ad+dd)))(a+b)", {"aa", "ab", "ba", "bb", "ca", "cb", "dada", "dadb", "ddda", "dddb"}, 0 });
	tests.push_back({ "(a+b)*", {"eps", "a", "b", "aa", "ab", "ba", "bb", "aaa", "aab", "aba", "abb", "baa", "bab", "bba", "bbb"}, 3 });
	tests.push_back({ "a(b+c+d)a", {"aba", "aca", "ada"}, 0 });
	tests.push_back({ "a*b*c", {"c", "bc", "bbc", "ac", "abc", "abbc", "aac", "aabc", "aabbc"}, 2 });
	tests.push_back({ "(a+b)(c+d)", {"ac", "ad", "bc", "bd"}, 0 });
	tests.push_back({ "a(bc)*d", {"ad", "abcd", "abcbcd"}, 2 });
	tests.push_back({ "a^+", {"a", "aa", "aaa", "aaaa"}, 4 });
	tests.push_back({ "a*b^+c*", {"b", "ab", "bc", "abc"}, 1 });
	return tests;
}