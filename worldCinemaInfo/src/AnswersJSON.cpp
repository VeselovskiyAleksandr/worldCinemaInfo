#pragma once
#include "AnswersJSON.h"

void AnswersJSON::searchAnswerFunction(multimap<string, vector< Entry>>& countWordsMap, map <size_t, vector< string>> getRequest)
{
	vector<string> requestWord, reqtWord;
	int reqNumber = 1;
	nlohmann::json dataConfig[MAX_RESPONS], docConfig, requestNumberConfig, requestResultConfig, answersResultConfig;
	vector< nlohmann::json> resultVectorConfig, answersVectorConfig, numRequestConfig;
	for (auto iterator = getRequest.begin(); iterator != getRequest.end(); iterator++)
	{
		for (auto& wordsRequest : iterator->second)
		{
			reqtWord.push_back(wordsRequest);
		}
		reqtWord.push_back(";");
		converterJSON.wordSplitFunction(reqtWord, requestWord);
		reqtWord.clear();
		multimap<size_t, size_t> searchResult;
		size_t absoluteRelevance = 0,
			maxAbsoluteRelevanceDoc = 0,
			maxAbsoluteRelevance = 0,
			nlohmArrayCount = 0;
		for (int i = 0; i < requestWord.size(); ++i)
		{
			for (auto it = countWordsMap.begin(); it != countWordsMap.end(); ++it)
			{
				if (requestWord[i] == it->first)
				{
					for (auto m : it->second)
					{
						searchResult.insert({ m.freqWordsCount, m.docId });
						absoluteRelevance += m.freqWordsCount;
						if (absoluteRelevance > maxAbsoluteRelevance)
							maxAbsoluteRelevanceDoc = absoluteRelevance;
					}
				}
			}
			int countResponses = 0;
			for (auto iter = searchResult.crbegin(); iter != searchResult.crend(); ++iter)
			{
				if (countResponses < MAX_RESPONS)
				{
					if (iter->first > 0)
					{
						docConfig += { { "Document number", iter->second }, { "Number of matches",iter->first }, };
					}
				}
				else
				{
					searchResult.clear();
					break;
				}
				countResponses++;
			}
			if (absoluteRelevance > 0)
				dataConfig[i] = { { "word ", requestWord[i]},{"result", { absoluteRelevance > 0} }, {"Absolute relevance", absoluteRelevance},{"Relative relevance",0.}, { docConfig}};
			if (absoluteRelevance == 0)
			{
				dataConfig[i] = { { "word ", requestWord[i]}, {"result", { absoluteRelevance != 0}}, {"Absolute relevance", absoluteRelevance},{"Relative relevance",0.}, { docConfig == 0}};
			}
			docConfig.clear();
			absoluteRelevance = 0;
			searchResult.clear();
			if (i == requestWord.size() - 1)
				requestWord.clear();
			if (maxAbsoluteRelevanceDoc > maxAbsoluteRelevance)
				maxAbsoluteRelevance = maxAbsoluteRelevanceDoc;
		}
		double relativeRelevance = 0;
		for (int id = 0; id < MAX_RESPONS; ++id)
		{
			for (const auto& item : dataConfig[id].items())
			{
				relativeRelevance = (double)dataConfig[id][2].at(1) / (double)maxAbsoluteRelevance;
				relativeRelevance = round(relativeRelevance * 100) / 100;
				dataConfig[id][3].at(1) = relativeRelevance;
			}
			if (dataConfig[id] > 0)
			{
				requestResultConfig += { {dataConfig[id]}};
			}
			dataConfig[id].clear();
		}
		requestNumberConfig = { {reqNumber, {requestResultConfig} } };
		resultVectorConfig.push_back(requestNumberConfig);
		requestResultConfig.clear();
		requestNumberConfig.clear();
		searchResult.clear();
		++reqNumber;
	}
	answersVectorConfig.push_back({ "Answers", resultVectorConfig });
	ofstream ansfile("answers.json", ios::out | ios::trunc);
	if (!ansfile)
		cout << "\n " << "Requests file is not found.";
	ansfile << answersVectorConfig;
	ansfile.close();
}

void AnswersJSON::getAnswerFunction()
{
	ifstream ansfile("answers.json");
	if (!ansfile)
		cout << "\n " << "Requests file is not found.";
	if (ansfile.peek() != EOF)
	{
		nlohmann::json ansconfig;
		int nReq = 0;
		string val = "", key = "";
		vector<string> vecAnswer;
		if (ansfile.peek() != EOF)
		{
			ansfile.seekg(0);
			ansfile >> ansconfig;
			ansfile.close();
			for (const auto& item : ansconfig.items())
			{
				for (const auto& ir : item.value().items())
				{
					for (const auto& iq : ir.value().items())
					{
						for (const auto& it : iq.value().items())
						{
							for (const auto& im : it.value().items())
							{
								if (nReq % 2 > 0)
								{
									key = to_string(im.value());
									vecAnswer.push_back(key);
									key = "";
								}
								if (nReq % 2 == 0)
								{
									val = to_string(im.value());
									vecAnswer.push_back(val);
									val = "";
								}
								nReq++;
							}
						}
					}
				}
			}
		}
		for (int i = 0; i < vecAnswer.size(); ++i)
		{
			cout << "\n " << vecAnswer[i];
		}
	}
}