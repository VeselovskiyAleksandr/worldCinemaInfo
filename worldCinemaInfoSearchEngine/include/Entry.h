#pragma once
#include <string>

struct Entry
{
	size_t docId, freqWordsCount;
	size_t wordFrequency;
	string word;
	bool checkAction;
};