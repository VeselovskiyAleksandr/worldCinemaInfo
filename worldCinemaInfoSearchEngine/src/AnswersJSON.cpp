#include "AnswersJSON.h"

void AnswersJSON::searchIdenticalWordsFunction(multimap<string, vector< Entry>>& countWordsMap, multimap<size_t, size_t>& searchResult,
                                               vector<string>& requestWord, size_t& absoluteRelevance, size_t& maxAbsoluteRelevance,
	                                           size_t& maxAbsoluteRelevanceDoc, int& i, multimap < size_t, vector <size_t>>& getDataRequest,
	                                           multimap < string, multimap <size_t, size_t>>& dataWord)
{
	size_t wordRepetition = 0;
	vector <size_t> docWord;
	auto itr = countWordsMap.equal_range(requestWord[i]);
	for (auto it = itr.first; it != itr.second; ++it)
	{
		for (auto m : it->second)
		{
			searchResult.insert({ m.freqWordsCount, m.docId });
			size_t wordRepetition = m.wordFrequency;
			docWord.push_back(m.docId);
		}
	}
	getDataRequest.insert({ wordRepetition, docWord });
	dataWord.insert({ requestWord[i],searchResult });
	wordRepetition = 0;
	docWord.clear();
}

void AnswersJSON::writingDataFileFunction(vector< nlohmann::json>& resultVectorConfig)
{
	ofstream ansfile("answers.json", ios::out | ios::trunc);
	if (!ansfile)
		cout << "\n " << "Answers file is not found.";
	ansfile << resultVectorConfig;
	ansfile.close();
	resultVectorConfig.clear();
}
void AnswersJSON::responseOutputFunction(vector<string>& vecAnswer)
{
	for (int i = 0; i < vecAnswer.size(); ++i)
	{
		cout << "\n " << vecAnswer[i];
	}
}