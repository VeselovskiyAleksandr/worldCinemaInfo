#include "InvertedIndex.h"

void InvertedIndex::vectorTraversalFunction(vector<string>& sentence, vector<string>& setWords, string word)
{
	for (int ir = 0; ir < sentence.size(); ++ir)
	{
		for (int n = 0; n < sentence[ir].length(); ++n)
		{
			wordEntryConditionFunction(word, sentence, setWords, ir, n);
		}
	}

}

template<typename Iterator>
void InvertedIndex::wordEntryConditionFunction(string& word, vector<string>& sentence, vector<string>& setWords, Iterator& ir, Iterator& n)
{
	if (sentence[ir][n] != '?' && sentence[ir][n] != '!' && sentence[ir][n] != '"' && sentence[ir][n] != '-' && sentence[ir][n] != '\'' && sentence[ir][n] != '/' &&
		sentence[ir][n] != ')' && sentence[ir][n] != '(' && sentence[ir][n] != ' ' && sentence[ir][n] != ';' && sentence[ir][n] != ':' && sentence[ir][n] != ',' &&
		sentence[ir][n] != '.')
	{
		if (sentence[ir][n] >= 65 && sentence[ir][n] <= 90)
		{
			sentence[ir][n] += 32;
		}
		word += sentence[ir][n];
	}
	else
	{
		if (word.length() > configuration.minWordLength)
			setWords.push_back(word);

		word = "";
	}
	if (ir == sentence[ir].length() - 1)
	{
		setWords.push_back(word);
		word = "";
	}
}

void InvertedIndex::wordSplitFunction(vector<string>& sentence, vector<string>& setWords)
{
	string highlightedWord = "";
	vectorTraversalFunction(sentence, setWords, highlightedWord);
}


void InvertedIndex::vectorEntryFillFunction(vector<string> vectWord[DOCUMENT_NUMBER], int filmId, vector<Entry> getCountWords[DOCUMENT_NUMBER])
{
	size_t wordRepetitionCount = 0;
	vector<string> checkRepetition;
	for (int itr = 0; itr < vectWord[filmId].size(); ++itr)
	{
		for (int iter = itr + 1; iter < vectWord[filmId].size(); ++iter)
		{
			if (vectWord[filmId][itr] == vectWord[filmId][iter])
			{
				wordRepetitionCount++;
			}
		}
		wordRepetitionCount++;
		Entry entry;
		auto pointer = find(checkRepetition.begin(), checkRepetition.end(), vectWord[filmId][itr]);
		if ((pointer == end(checkRepetition)) && (vectWord[filmId][itr].size() > configuration.minWordLength))
		{
			entry.freqWordsCount = wordRepetitionCount;
			entry.docId = filmId;
			entry.word = vectWord[filmId][itr];
			entry.checkAction = false;
			getCountWords[filmId].push_back(entry);
		}
		checkRepetition.push_back(vectWord[filmId][itr]);
		wordRepetitionCount = 0;
	}
	checkRepetition.clear();
}

void InvertedIndex::relevanceIncreasingFunction(vector <size_t> dIDVector, multimap < string, multimap <size_t, size_t >>& dataWord, int& i, size_t& requestAbsoluteRelevance)
{
	for (auto& it = dataWord.begin(); it != dataWord.end(); ++it)
	{
		for (const auto& iq : it->second)
		{
			if (iq.second == dIDVector[i])
				requestAbsoluteRelevance += iq.first;
		}
	}
}

void InvertedIndex::maxRelevanceFindingFunction(vector <size_t> dIDVector, multimap < string, multimap <size_t, size_t >>& dataWord, size_t& requestAbsoluteRelevance,
	                                            size_t& maxRequestAbsoluteRelevance, multimap <size_t, size_t >& searchRequestResult)
{
	for (int i = 0; i < dIDVector.size(); ++i)
	{
		relevanceIncreasingFunction(dIDVector, dataWord, i, requestAbsoluteRelevance);
		if (requestAbsoluteRelevance > maxRequestAbsoluteRelevance)
			maxRequestAbsoluteRelevance = requestAbsoluteRelevance;
		searchRequestResult.insert({ requestAbsoluteRelevance, dIDVector[i] });
		requestAbsoluteRelevance = 0;
	}
}

template<typename Iterator>
void InvertedIndex::fillEntryFunction(size_t& countPosition, int& i, vector<Entry> getCountWords[DOCUMENT_NUMBER], Iterator& it, size_t& count, Entry& entry)
{
	entry.docId = it.docId;
	entry.word = it.word;
	entry.wordFrequency = count;
	entry.checkAction = true;
	getCountWords[i][countPosition] = entry;
}

template<typename Iterator>
void InvertedIndex::matchSearchInnerLoopFunction(int& i, vector<Entry> getCountWords[DOCUMENT_NUMBER], Iterator& it, size_t& count)
{
	for (int j = i + 1; j != DOCUMENT_NUMBER; ++j)
	{
		size_t countPos_2 = 0;
		for (auto& qj : getCountWords[j])
		{
			if ((it.word == qj.word) && (qj.checkAction != true))
			{
				count += qj.freqWordsCount;
				Entry entry;
				fillEntryFunction(countPos_2, i, getCountWords, it, count, entry);
				break;
			}
			countPos_2++;
		}
	}
}

void InvertedIndex::matchSearchOuterLoopFunction(vector<Entry> getCountWords[DOCUMENT_NUMBER], vector <Entry>& vectEntry, multimap<string, vector< Entry>>& countWordsMap)
{
	for (int i = 0; i != DOCUMENT_NUMBER; ++i)
	{
		size_t countPos_1 = 0;
		for (auto& it : getCountWords[i])
		{
			size_t count = it.freqWordsCount;
			if (it.checkAction == true)
				break;
			matchSearchInnerLoopFunction(i, getCountWords, it, count);
			Entry entry;
			entry.freqWordsCount = it.freqWordsCount;
			fillEntryFunction(countPos_1, i, getCountWords, it, count, entry);
			vectEntry.push_back(entry);
			countWordsMap.insert({ it.word, vectEntry });
			vectEntry.clear();
			count = 0;
			++countPos_1;
		}
	}
}

void InvertedIndex::transferElementsFunction(vector <size_t>& docIDVector, vector <size_t>& dIDVector)
{
	if (docIDVector.size() > 0)
	{
		for (int i = 0; i < docIDVector.size(); ++i)
		{
			dIDVector.push_back(docIDVector[i]);
		}
	}
}