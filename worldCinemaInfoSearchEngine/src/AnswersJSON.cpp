
#include "AnswersJSON.h"

void AnswersJSON::searchIdenticalWordsFunction(multimap<string, vector< Entry>>& countWordsMap, multimap<size_t, size_t>& searchResult,
	                                           vector<string> &requestWord, size_t& absoluteRelevance, size_t& maxAbsoluteRelevance,
	                                           size_t& maxAbsoluteRelevanceDoc, int& i, multimap < size_t, vector <size_t>>&  getDataRequest,
	                                           multimap < string, multimap <size_t, size_t>>& dataWord)

{
	size_t wordRepetition = 0;
	vector <size_t> docWord;
	for (auto it = countWordsMap.begin(); it != countWordsMap.end(); ++it)
	{
		if (requestWord[i] == it->first)
		{
			for (auto m : it->second)
			{
				searchResult.insert({ m.freqWordsCount, m.docId });
				size_t wordRepetition = m.wordFrequency;
				docWord.push_back(m.docId);
			}
		}
	}
	getDataRequest.insert({ wordRepetition, docWord});
	dataWord.insert({ requestWord[i],searchResult});
	wordRepetition = 0;
	docWord.clear();
}

template<typename Iterator>
void AnswersJSON::nullResultRecordingFunction(vector <size_t>& docIDVector_1, vector <size_t>& docIDVector_2, nlohmann::json& requestNumberConfig, 
	                                          nlohmann::json requestConfig[MAX_RESPONS], Iterator& iterator, int& countReqResponses, nlohmann::json& docConfig)
{
	requestNumberConfig = { {{{ "Request",{ iterator->first + 1, iterator->second }}}} };
	requestConfig[countReqResponses] = { {docConfig, { "Absolute relevance", 0 },
    {"Relative relevance", 0.}} };
	docIDVector_1.clear();
	docIDVector_2.clear();
}

template<typename Iterat, typename Iter>
void AnswersJSON::vectorTraversalFunction(vector <size_t>& docIDVector_1, vector <size_t>& docIDVector_2, bool ifDoc, nlohmann::json& requestNumberConfig,
	nlohmann::json requestConfig[MAX_RESPONS], int& countReqResponses, Iterat& iterator, Iter& iter)
{
	for (int im = 0; im != docIDVector_1.size(); ++im)
	{
		for (auto it : iter->second)
		{
			if (docIDVector_1[im] == it)
			{
				ifDoc = true;
				docIDVector_2.push_back(docIDVector_1[im]);
			}
		}
		if (ifDoc == false)
		{
			nlohmann::json docConfig;
			docConfig = { { "result",  ifDoc } };
			nullResultRecordingFunction(docIDVector_1, docIDVector_2, requestNumberConfig, requestConfig, iterator, countReqResponses, docConfig);
			break;
		}
		ifDoc = false;
	}
	docIDVector_1.clear();
}

void AnswersJSON::transferElementsFunction(vector <size_t>& docIDVector, vector <size_t>& dIDVector)
{
	if (docIDVector.size() > 0)
	{
		for (int i = 0; i < docIDVector.size(); ++i)
		{
			dIDVector.push_back(docIDVector[i]);
		}
	}
}

template<typename Iterator>
void AnswersJSON::contentLeastFrequentWordFunction(Iterator& iter, multimap < size_t, vector <size_t>>& getDataRequest, vector <size_t>& docIDVector)
{
	if (iter == getDataRequest.begin())
	{
		for (auto ip : iter->second)
		{
			docIDVector.push_back(ip);
		}
	}
}

void AnswersJSON::findingRequestDataFunction(multimap <size_t, size_t >& searchRequestResult, double& relativeReqRelevance, size_t& maxRequestAbsoluteRelevance, 
	                                        nlohmann::json requestConfig[MAX_RESPONS], int& countReqResponses, bool& ifDoc, nlohmann::json requestNumberConfig)
{
	nlohmann::json docConfig;
	for (auto iter = searchRequestResult.crbegin(); iter != searchRequestResult.crend(); ++iter)
	{
		docConfig = { {"docID ", iter->second }, { "result",  ifDoc } };
		relativeReqRelevance = (double)(iter->first) / (double)maxRequestAbsoluteRelevance;
		relativeReqRelevance = round(relativeReqRelevance * 100) / 100;
		requestConfig[countReqResponses] ={ { docConfig, {{ "Absolute relevance", iter->first }, {"Relative relevance", relativeReqRelevance}} }};
		countReqResponses++;
		if (countReqResponses == MAX_RESPONS)
			break;
	}
	docConfig.clear();
}

void AnswersJSON::cleaningJSONArrayFunction(nlohmann::json requestConfig[MAX_RESPONS])
{
	for (int i = 0; i < MAX_RESPONS; ++i)
	{
		requestConfig[i].clear();
	}
}

void AnswersJSON::relevanceIncreasingFunction(vector <size_t> dIDVector, multimap < string, multimap <size_t, size_t >> &dataWord, int& i, size_t& requestAbsoluteRelevance)
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

template<typename Iterator>
void AnswersJSON::findingDocumentsRequestFunction(multimap < size_t, vector <size_t>>& getDataRequest, vector <size_t>& docIDVector, vector <size_t>& dIDVector,
	nlohmann::json requestConfig[MAX_RESPONS], nlohmann::json& requestNumberConfig, bool& ifDoc, int& countReqResponses, Iterator& iterator)
{
	for (auto& iter = getDataRequest.begin(); iter != getDataRequest.end(); ++iter)
	{
		contentLeastFrequentWordFunction(iter, getDataRequest, docIDVector);
		if ((dIDVector.size()) == 0 && (docIDVector.size() > 0))
		{
			vectorTraversalFunction(docIDVector, dIDVector, ifDoc, requestNumberConfig, requestConfig, countReqResponses, iterator, iter);
		}
		else if ((docIDVector.size()) == 0 && (dIDVector.size() > 0))
		{
			vectorTraversalFunction(dIDVector, docIDVector, ifDoc, requestNumberConfig, requestConfig, countReqResponses, iterator, iter);
		}
	}
	transferElementsFunction(docIDVector, dIDVector);
	getDataRequest.clear();
}

void AnswersJSON::maxRelevanceFindingFunction(vector <size_t> dIDVector, multimap < string, multimap <size_t, size_t >>& dataWord, size_t& requestAbsoluteRelevance,
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
void AnswersJSON::searchRequestRelevanceFunction(multimap < size_t, vector <size_t>>& getDataRequest, vector< nlohmann::json>& resultVectorConfig, Iterator& iterator,
	multimap < string, multimap <size_t, size_t>>& dataWord, size_t& requestAbsoluteRelevance, size_t& maxRequestAbsoluteRelevance, multimap <size_t, size_t >& searchRequestResult)
{
	nlohmann::json requestConfig[MAX_RESPONS], requestNumberConfig, requestResultConfig;
	vector <size_t> docIDVector, dIDVector;
	bool ifDoc = false;
	int countReqResponses = 0;
	double relativeReqRelevance = 0;
	findingDocumentsRequestFunction(getDataRequest, docIDVector, dIDVector, requestConfig, requestNumberConfig, ifDoc, countReqResponses, iterator);
	if (docIDVector.size() > 0 || dIDVector.size() > 0)
		ifDoc = true;
	maxRelevanceFindingFunction(dIDVector, dataWord, requestAbsoluteRelevance, maxRequestAbsoluteRelevance, searchRequestResult);
	dIDVector.clear();
	docIDVector.clear();
	requestNumberConfig = { {{{ "Request",{ iterator->first + 1, iterator->second }}}} };
	findingRequestDataFunction(searchRequestResult, relativeReqRelevance, maxRequestAbsoluteRelevance, requestConfig, countReqResponses, ifDoc, requestNumberConfig);
	requestResultConfig = { { requestNumberConfig},{ requestConfig} };
	resultVectorConfig.push_back(requestResultConfig);
	cleaningJSONArrayFunction(requestConfig);
	requestNumberConfig.clear();
	requestResultConfig.clear();
	maxRequestAbsoluteRelevance = 0;
	requestAbsoluteRelevance = 0;
	countReqResponses = 0;
}

void AnswersJSON::queryProcessingFunction(vector<string>& requestWord, multimap<size_t, size_t>& searchResult,  multimap<string, vector< Entry>>& countWordsMap, nlohmann::json& docConfig,
	                                      nlohmann::json dataConfig[MAX_RESPONS], size_t& absoluteRelevance, size_t& maxAbsoluteRelevance, size_t& maxAbsoluteRelevanceDoc,
	                                      multimap < size_t, vector <size_t>> & getDataRequest,  multimap < string, multimap <size_t, size_t>>& dataWord)
{
	for (int i = 0; i < requestWord.size(); ++i)
	{
			int countResponses = 0;
			searchIdenticalWordsFunction(countWordsMap, searchResult, requestWord, absoluteRelevance, maxAbsoluteRelevance, maxAbsoluteRelevanceDoc, i, getDataRequest, dataWord);
			docConfig.clear();
			absoluteRelevance = 0;
			searchResult.clear();
			if (i == requestWord.size() - 1)
				requestWord.clear();
			if (maxAbsoluteRelevanceDoc > maxAbsoluteRelevance)
				maxAbsoluteRelevance = maxAbsoluteRelevanceDoc;
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

template<typename Iterator>
void AnswersJSON::requestProcessingFunction(Iterator& iterator, multimap<string, vector< Entry>>& countWordsMap, vector<string>& requestWord, vector<string>& reqtWord,
	                                        int& reqNumber, nlohmann::json dataConfig[MAX_RESPONS], nlohmann::json docConfig, nlohmann::json requestNumberConfig,
                                          	nlohmann::json requestResultConfig, vector< nlohmann::json>& resultVectorConfig)
{
	multimap < size_t, vector <size_t>>  getDataRequest;
	multimap < string, multimap <size_t, size_t>> dataWord;
	multimap <size_t, size_t > searchRequestResult, searchResult;
	size_t absoluteRelevance = 0, maxAbsoluteRelevanceDoc = 0, maxAbsoluteRelevance = 0, nlohmArrayCount = 0;
	size_t  requestAbsoluteRelevance = 0, maxRequestAbsoluteRelevance = 0;
	double relativeRelevance = 0;
	writingQueriesVectorFunction(reqtWord, iterator);
	converterJSON.wordSplitFunction(reqtWord, requestWord);
	reqtWord.clear();
	queryProcessingFunction(requestWord, searchResult, countWordsMap, docConfig, dataConfig, absoluteRelevance, maxAbsoluteRelevance, maxAbsoluteRelevance, getDataRequest, dataWord);
	searchRequestRelevanceFunction(getDataRequest, resultVectorConfig, iterator, dataWord, requestAbsoluteRelevance, maxRequestAbsoluteRelevance, searchRequestResult);
	requestNumberConfig = { {reqNumber, {requestResultConfig} } };
	resultVectorConfig.push_back(requestNumberConfig);
	requestResultConfig.clear();
	requestNumberConfig.clear();
	searchResult.clear();
	++reqNumber;
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

void AnswersJSON::searchAnswerFunction(multimap<string, vector< Entry>>& countWordsMap, map <size_t, vector< string>> getRequest)
{
	vector<string> requestWord, reqtWord;
	int reqNumber = 1;
	nlohmann::json dataConfig[MAX_RESPONS], docConfig, requestNumberConfig, requestResultConfig;
	vector< nlohmann::json> resultVectorConfig;
	for (auto iterator = getRequest.begin(); iterator != getRequest.end(); iterator++)
	{
		requestProcessingFunction(iterator, countWordsMap, requestWord, reqtWord, reqNumber, dataConfig, docConfig, requestNumberConfig, requestResultConfig, resultVectorConfig);
	}
	writingDataFileFunction(resultVectorConfig);
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

void AnswersJSON::responseOutputFunction(vector<string>& vecAnswer)
{
	for (int i = 0; i < vecAnswer.size(); ++i)
	{
		cout << "\n " << vecAnswer[i];
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