#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <regex>
#include "UnitTest.hpp"
using namespace std;

class Set
{
	list<string> elements;
public:
	Set() {}
	Set(list<string> elements) : elements(elements) {}
	static Set iterations(const Set& set, int depth, bool isZero = true);
	static Set intersect(const Set& fset, const Set& sset);
	static Set unification(const Set& fset, const Set& sset);
	list<string>& getSeq();
	void sortSeq();
};

class SetBuilder
{
	enum Operations
	{
		ZITERATION, OITERATION, INTERSECT, UNIFICATION
	} typedef OS;
	struct SetInfo
	{
		SetInfo(Set set, int position) : set(set), position(position) {}
		Set set;
		int position = 0;
	};
	struct OperationInfo
	{
		OperationInfo(int position, OS operation) : position(position), operation(operation) {}
		bool operator > (const OperationInfo& oi)
		{
			return operation > oi.operation;
		}
		OS operation = INTERSECT;
		int position = 0;
	};
	void identifyOS(const string& matched, const int& position, list<OperationInfo>& operations);
	Set execOS(vector<SetInfo>& sets, list<OperationInfo>& operations, const int& depth);
	list<string> brackDistr(const string& matched);
public:
	Set decrypte(string formula, int depth);
	static void doTest();
};
