#include "Regulars.h"
using namespace std;

Set Set::iterations(const Set& set, int depth, bool isZero)
{
	Set buildSet;
	if (isZero)
		buildSet.elements.push_back("eps");
	Set tempSet{ {"eps"} };
	for (int i = 0; i < depth; ++i)
	{
		Set prebuildSet;
		for (auto& seq : set.elements)
			prebuildSet = unification(prebuildSet, intersect(Set({ seq }), tempSet));
		tempSet = prebuildSet;
		buildSet = unification(buildSet, tempSet);
	}
	return buildSet;
}

Set Set::intersect(const Set& fset, const Set& sset)
{
	Set buildSet;
	for (auto& fseq : fset.elements)
		for (auto& sseq : sset.elements)
			if (fseq == "eps" && sseq == "eps")
			{
				if (find(buildSet.elements.cbegin(), buildSet.elements.cend(), "eps") == buildSet.elements.cend())
					buildSet.elements.push_back("eps");
			}
			else if (fseq == "eps")
			{
				if (find(buildSet.elements.cbegin(), buildSet.elements.cend(), sseq) == buildSet.elements.cend())
					buildSet.elements.push_back(sseq);
			}
			else if (sseq == "eps")
			{
				if (find(buildSet.elements.cbegin(), buildSet.elements.cend(), fseq) == buildSet.elements.cend())
					buildSet.elements.push_back(fseq);
			}
			else
			{
				if (find(buildSet.elements.cbegin(), buildSet.elements.cend(), fseq + sseq) == buildSet.elements.cend())
					buildSet.elements.push_back(fseq + sseq);
			}
	return buildSet;
}

Set Set::unification(const Set& fset, const Set& sset)
{
	Set buildSet;
	for (auto& fseq : fset.elements)
		buildSet.elements.push_back(fseq);
	for (auto& sseq : sset.elements)
	{
		if (find(buildSet.elements.cbegin(), buildSet.elements.cend(), sseq) == buildSet.elements.cend())
			buildSet.elements.push_back(sseq);
	}
	return buildSet;
}

list<string>& Set::getSeq()
{
	return elements;
}

void Set::sortSeq()
{
	elements.sort([](const string& fir, const string& sec) {return fir.size() < sec.size(); });
}

void SetBuilder::identifyOS(const string& matched, const int& position, list<OperationInfo>& operations)
{
	regex regOperations("(\\^\\+)|\\*|\\+");
	bool hasPlus = false;
	sregex_iterator startOS(matched.cbegin(), matched.cend(), regOperations);
	while (startOS != sregex_iterator())
	{
		if (startOS->str() == "^+")
			operations.push_back({ position, OS::OITERATION });
		else if (startOS->str() == "*")
			operations.push_back({ position, OS::ZITERATION });
		else if (startOS->str() == "+")
		{
			hasPlus = true;
			operations.push_back({ position, OS::UNIFICATION });
		}
		++startOS;
	}
	if (!hasPlus)
		operations.push_back({ position, OS::INTERSECT });
}

Set SetBuilder::execOS(vector<SetInfo>& sets, list<OperationInfo>& operations, const int& depth)
{
	operations.sort([](const OperationInfo& f, const OperationInfo& s) {return f.operation < s.operation; });
	for (auto& op : operations)
	{
		switch (op.operation)
		{
		case OS::ZITERATION:
		{
			sets[op.position].set = Set::iterations(sets[op.position].set, depth);
			break;
		}
		case OS::OITERATION:
		{
			sets[op.position].set = Set::iterations(sets[op.position].set, depth, false);
			break;
		}
		case OS::INTERSECT:
		{
			if (op.position != sets.size() - 1)
			{
				sets[op.position].set = Set::intersect(sets[op.position].set, sets[op.position + 1].set);
				sets.erase(sets.begin() + op.position + 1);
				for (auto& iset : sets)
					if (iset.position > op.position)
						--iset.position;
				for (auto& sop : operations)
					if (sop.position > op.position)
						--sop.position;
			}
			break;
		}
		case OS::UNIFICATION:
		{
			if (op.position != sets.size() - 1)
			{
				sets[op.position].set = Set::unification(sets[op.position].set, sets[op.position + 1].set);
				sets.erase(sets.begin() + op.position + 1);
				for (auto& sop : operations)
					if (sop.position > op.position)
						--sop.position;
			}
			break;
		}
		default:
			break;
		}
	}
	return sets[0].set;
}

list<string> SetBuilder::brackDistr(const string& matched)
{
	list<string> brackets;
	int brCount = 0;
	auto lastBracket = 0;
	for (size_t i = 0; i < matched.size();)
	{
		if (matched[i] == '(')
			++brCount;
		else if (matched[i] == ')')
		{
			--brCount;
			if (brCount == 0)
			{
				++i;
				while (i < matched.size() && matched[i] != ')' && matched[i] != '(')
					++i;
				brackets.push_back(matched.substr(lastBracket, i - lastBracket));
				lastBracket = i;
				continue;
			}
		}
		++i;
	}
	return brackets;
}

Set SetBuilder::decrypte(string formula, int depth)
{
	regex regExpressions("(\\(.*\\)((\\^\\+{1,2})|\\+|\\*|(\\*\\+))?)|([a-zA-Z0-9](\\+|(\\^\\+{1,2})|(\\*\\+?))?)");
	regex regBrackets("\\((.*)\\)");
	Set set;
	vector<SetInfo> sets;
	list<OperationInfo> operations;
	int position = 0;
	sregex_iterator start(formula.cbegin(), formula.cend(), regExpressions);
	while (start != sregex_iterator())
	{
		string matched = start->str();
		if (regex_search(matched, regBrackets))
		{
			list<string> brackets = brackDistr(matched);
			for (auto& brCon : brackets)
			{
				sets.push_back({ decrypte(brCon.substr(1, brCon.rfind(')') - 1), depth), position });
				string brOS = brCon.substr(brCon.rfind(')') + 1, brCon.size() - brCon.rfind(')') - 1);
				if (brOS.size() != 0)
					identifyOS(brOS, position, operations);
				else
					operations.push_back({ position, OS::INTERSECT });
				++position;
			}
		}
		else
		{
			sets.push_back({ {{matched.substr(0,1)}}, position });
			if (matched.size() != 1)
				identifyOS(matched, position, operations);
			else
				operations.push_back({ position, OS::INTERSECT });
			++position;
		}
		++start;
	}
	return execOS(sets, operations, depth);
}

void SetBuilder::doTest()
{
#ifdef AVAILABLE_UNIT_TEST
	SetBuilder setBuilder;
	list<UnitTest> units = unitTests();
	int count = 1;
	for (auto& unit : units)
	{
		int seqCoincidence = 0;
		Set testSet = setBuilder.decrypte(unit.formula, unit.depth);;
		for (auto& seq : unit.seq)
		{
			if (find(testSet.getSeq().cbegin(), testSet.getSeq().cend(), seq) != testSet.getSeq().cend())
				++seqCoincidence;
			else
				cout << seq << endl;
		}
		if (seqCoincidence == unit.seq.size() && unit.seq.size() == testSet.getSeq().size())
			cout << "\nUnit test #" << count++ << " succeed!";
		else
			cout << "\nUnit test #" << count++ << " failed!";
	}
#endif // AVAILABLE_UNIT_TEST
#ifndef AVAILABLE_UNIT_TEST
	cout << "\nUnits test not loaded!!!";
#endif // !AVAILABLE_UNIT_TEST

}

int main()
{
#ifdef _DEBUG
	SetBuilder::doTest();
#endif // _DEBUG
#ifndef _DEBUG
	regex iteration("\\*");
	SetBuilder setBuilder;
	bool run = true;
	while (run)
	{
		cout << "\nChoose option:";
		cout << "\n[0] Exit\n[1] Build regular set\nChoise: ";
		int choise = 0;
		cin >> choise;
		switch (choise)
		{
		case 0:
		{
			run = false;
			break;
		}
		case 1:
		{
			string formula = "";
			int depth = 0;
			bool hasEps = false;
			cout << "\nEnter formula: ";
			cin.get();
			getline(cin, formula);
			cout << "\nEnter depth of iteration (0 - always epsilon): ";
			cin >> depth;
			list<string> sequences = setBuilder.decrypte(formula, depth).getSeq();
			auto iter = find(sequences.begin(), sequences.end(), "eps");
			if (iter != sequences.end())
			{
				hasEps = true;
				sequences.erase(iter);
			}
			sequences.sort([](const string& f, const string& s) {return f.size() < s.size(); });
			if (hasEps)
				sequences.push_front("eps");
			cout << "\nSet: { ";
			int i = 0;
			for (auto& seq : sequences)
			{
				cout << seq;
				if (i != sequences.size() - 1)
					cout << ", ";
				++i;
			}
			if (regex_search(formula, iteration))
				cout << ", ... ";
			else
				cout << " ";
			cout << "}";
		}
		default:
			break;
		}
	}
#endif // !_DEBUG
	return EXIT_SUCCESS;
}