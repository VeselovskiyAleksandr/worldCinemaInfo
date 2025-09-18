#pragma once
#include "AnswersJSON.h"

void AnswersJSON::searchIdenticalWordsFunction(multimap<string, vector< Entry>>& countWordsMap, multimap<size_t, size_t>& searchResult,
	                                           vector<string> &requestWord, size_t& absoluteRelevance, size_t& maxAbsoluteRelevance,
	                                           size_t& maxAbsoluteRelevanceDoc, int& i)
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
}

void AnswersJSON::recordNumberRepetitionsFunction(multimap<size_t, size_t> &searchResult, int& countResponses, nlohmann::json& docConfig)
{
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
}

template<typename JsonIterator>
void AnswersJSON::writingQueriesVectorFunction(vector<string>& reqtWord, JsonIterator& iterator)
{
	for (auto& wordsRequest : iterator->second)
	{
		reqtWord.push_back(wordsRequest);
	}
	reqtWord.push_back(";");
}

void AnswersJSON::relevanceRecordFunction(nlohmann::json dataConfig[MAX_RESPONS], nlohmann::json& requestResultConfig,
	                                      double& relativeRelevance, size_t& maxAbsoluteRelevance)
{
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
}

void AnswersJSON::fillingDataJSONArrayFunction(nlohmann::json dataConfig[MAX_RESPONS], vector<string>& requestWord,
	                                           nlohmann::json docConfig, size_t absoluteRelevance, int i)
{
	if (absoluteRelevance > 0)
		dataConfig[i] = { { "word ", requestWord[i]},{"result", { absoluteRelevance > 0} }, {"Absolute relevance", absoluteRelevance},
			{"Relative relevance",0.}, { docConfig} };
	if (absoluteRelevance == 0)
	{
		dataConfig[i] = { { "word ", requestWord[i]}, {"result", { absoluteRelevance != 0}}, {"Absolute relevance", absoluteRelevance},
			{"Relative relevance",0.}, { docConfig == 0} };
	}
}


void AnswersJSON::queryProcessingFunction(vector<string>& requestWord, multimap<size_t, size_t>& searchResult,
	                                      multimap<string, vector< Entry>>& countWordsMap, nlohmann::json& docConfig,
	                                      nlohmann::json dataConfig[MAX_RESPONS], size_t& absoluteRelevance,
	                                      size_t& maxAbsoluteRelevance, size_t& maxAbsoluteRelevanceDoc)
{
	for (int i = 0; i < requestWord.size(); ++i)
	{
		int countResponses = 0;
		searchIdenticalWordsFunction(countWordsMap, searchResult, requestWord, absoluteRelevance, maxAbsoluteRelevance, maxAbsoluteRelevanceDoc, i);
		recordNumberRepetitionsFunction(searchResult, countResponses, docConfig);
		fillingDataJSONArrayFunction(dataConfig, requestWord, docConfig, absoluteRelevance, i);
		docConfig.clear();
		absoluteRelevance = 0;
		searchResult.clear();
		if (i == requestWord.size() - 1)
			requestWord.clear();
		if (maxAbsoluteRelevanceDoc > maxAbsoluteRelevance)
			maxAbsoluteRelevance = maxAbsoluteRelevanceDoc;
	}
}

void AnswersJSON::responseOutputFunction(vector<string>& vecAnswer)
{
	for (int i = 0; i < vecAnswer.size(); ++i)
	{
		cout << "\n " << vecAnswer[i];
	}
}

template<typename JsonType>
void AnswersJSON::extractDataFileFunction(JsonType& ansconfig, int nReq, string val, string key, vector<string>& vecAnswer)
{
	for (const auto& item : ansconfig.items())
	{
		for (const auto& ir : item.value().items())
		{
			JsonTraversalFunction(ir, nReq, val, key, vecAnswer);
		}
	}
}

void AnswersJSON::searchAnswerFunction(multimap<string, vector< Entry>>& countWordsMap, map <size_t, vector< string>> getRequest)
{
	vector<string> requestWord, reqtWord;
	int reqNumber = 1;
	nlohmann::json dataConfig[MAX_RESPONS], docConfig, requestNumberConfig, requestResultConfig, answersResultConfig;
	vector< nlohmann::json> resultVectorConfig, numRequestConfig;
	for (auto iterator = getRequest.begin(); iterator != getRequest.end(); iterator++)
	{
		multimap<size_t, size_t> searchResult;
		size_t absoluteRelevance = 0, maxAbsoluteRelevanceDoc = 0, maxAbsoluteRelevance = 0, nlohmArrayCount = 0;
		double relativeRelevance = 0;
		writingQueriesVectorFunction(reqtWord, iterator);
		converterJSON.wordSplitFunction(reqtWord, requestWord);
		reqtWord.clear();
		queryProcessingFunction(requestWord, searchResult, countWordsMap, docConfig, dataConfig, absoluteRelevance, 
			                    maxAbsoluteRelevance, maxAbsoluteRelevanceDoc);
		relevanceRecordFunction(dataConfig, requestResultConfig, relativeRelevance, maxAbsoluteRelevance);
		requestNumberConfig = { {reqNumber, {requestResultConfig} } };
		resultVectorConfig.push_back(requestNumberConfig);
		requestResultConfig.clear();
		requestNumberConfig.clear();
		searchResult.clear();
		++reqNumber;
	}
	ofstream ansfile("answers.json", ios::out | ios::trunc);
	if (!ansfile)
		cout << "\n " << "Requests file is not found.";
	ansfile << resultVectorConfig;
	ansfile.close();
}

template<typename JsonType>
void AnswersJSON::fillingAnswerVector(JsonType& it, int nReq, string val, string key, vector<string>& vecAnswer)
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

template<typename JsonType>
void AnswersJSON::JsonTraversalFunction(const JsonType& ir, int nReq, string val, string key, vector<string>& vecAnswer)
{
	for (const auto& iq : ir.value().items())
	{

		for (const auto& it : iq.value().items())
		{
				fillingAnswerVector(it, nReq, val, key, vecAnswer);
		}
	}
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
			extractDataFileFunction(ansconfig, nReq, val, key, vecAnswer);
		}
		responseOutputFunction(vecAnswer);
	}
}