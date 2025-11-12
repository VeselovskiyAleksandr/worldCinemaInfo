#include "ConverterJSON.h"

template<typename Iterator>
void ConverterJSON::nullResultRecordingFunction(vector <size_t>& docIDVector_1, vector <size_t>& docIDVector_2, nlohmann::json& requestNumberConfig,
	                                            nlohmann::json requestConfig[MAX_RESPONS], Iterator& iterator, int& countReqResponses, nlohmann::json& docConfig)
{
	requestConfig[countReqResponses] = { {docConfig, { "Absolute relevance", 0 }, {"Relative relevance", 0.}} };
	docIDVector_1.clear();
	docIDVector_2.clear();
}

template<typename Iterat, typename Iter>
void ConverterJSON::vectorTraversalFunction(vector <size_t>& docIDVector_1, vector <size_t>& docIDVector_2, bool ifDoc, nlohmann::json& requestNumberConfig,
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

template<typename Iterator>
void ConverterJSON::contentLeastFrequentWordFunction(Iterator& iter, multimap < size_t, vector <size_t>>& getDataRequest, vector <size_t>& docIDVector)
{
	if (iter == getDataRequest.begin())
	{
		for (auto ip : iter->second)
		{
			docIDVector.push_back(ip);
		}
	}
}

void ConverterJSON::findingRequestDataFunction(multimap <size_t, size_t >& searchRequestResult, double& relativeReqRelevance, size_t& maxRequestAbsoluteRelevance,
	                                           nlohmann::json requestConfig[MAX_RESPONS], int& countReqResponses, bool& ifDoc, nlohmann::json requestNumberConfig)
{
	nlohmann::json docConfig;
	for (auto iter = searchRequestResult.crbegin(); iter != searchRequestResult.crend(); ++iter)
	{
		docConfig = { {"docID ", iter->second }, { "result",  ifDoc } };
		relativeReqRelevance = (double)(iter->first) / (double)maxRequestAbsoluteRelevance;
		relativeReqRelevance = round(relativeReqRelevance * 100) / 100;
		requestConfig[countReqResponses] = { { docConfig, {{ "Absolute relevance", iter->first }, {"Relative relevance", relativeReqRelevance}} } };
		countReqResponses++;
		if (countReqResponses == MAX_RESPONS)
			break;
	}
	docConfig.clear();
}

template<typename Iterator>
void ConverterJSON::findingDocumentsRequestFunction(multimap < size_t, vector <size_t>>& getDataRequest, vector <size_t>& docIDVector, vector <size_t>& dIDVector,
	                                                nlohmann::json requestConfig[MAX_RESPONS], nlohmann::json& requestNumberConfig, bool& ifDoc, int& countReqResponses,
	                                                Iterator& iterator)
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
	invertedIndex.transferElementsFunction(docIDVector, dIDVector);
	getDataRequest.clear();
}

template<typename Iterator>
void ConverterJSON::searchRequestRelevanceFunction(multimap < size_t, vector <size_t>>& getDataRequest, vector< nlohmann::json>& resultVectorConfig, Iterator& iterator,
	                                               multimap < string, multimap <size_t, size_t>>& dataWord, size_t& requestAbsoluteRelevance, size_t& maxRequestAbsoluteRelevance, 
	                                               multimap <size_t, size_t >& searchRequestResult)
{
	nlohmann::json requestConfig[MAX_RESPONS], requestNumberConfig, requestResultConfig;
	vector <size_t> docIDVector, dIDVector;
	bool ifDoc = false;
	int countReqResponses = 0;
	double relativeReqRelevance = 0;
	findingDocumentsRequestFunction(getDataRequest, docIDVector, dIDVector, requestConfig, requestNumberConfig, ifDoc, countReqResponses, iterator);
	if (docIDVector.size() > 0 || dIDVector.size() > 0)
		ifDoc = true;
	invertedIndex.maxRelevanceFindingFunction(dIDVector, dataWord, requestAbsoluteRelevance, maxRequestAbsoluteRelevance, searchRequestResult);
	dIDVector.clear();
	docIDVector.clear();
	requestNumberConfig = { {{{ "Request",{ iterator->first+1, iterator->second }}}} };
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

void ConverterJSON::queryProcessingFunction(vector<string>& requestWord, multimap<size_t, size_t>& searchResult, multimap<string, vector< Entry>>& countWordsMap,
                                        	nlohmann::json& docConfig, nlohmann::json dataConfig[MAX_RESPONS], size_t& absoluteRelevance, size_t& maxAbsoluteRelevance,
	                                        size_t& maxAbsoluteRelevanceDoc, multimap < size_t, vector <size_t>>& getDataRequest, multimap < string, multimap <size_t, size_t>>& dataWord)
{
	for (int i = 0; i < requestWord.size(); ++i)
	{
		int countResponses = 0;
		answersJSON.searchIdenticalWordsFunction(countWordsMap, searchResult, requestWord, absoluteRelevance, maxAbsoluteRelevance, maxAbsoluteRelevanceDoc, i, getDataRequest, dataWord);
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
void ConverterJSON::writingQueriesVectorFunction(vector<string>& reqtWord, JsonIterator& iterator)
{
	for (auto& wordsRequest : iterator->second)
	{
		reqtWord.push_back(wordsRequest);
	}
	reqtWord.push_back(";");
}

template<typename Iterator>
void ConverterJSON::requestProcessingFunction(Iterator& iterator, multimap<string, vector< Entry>>& countWordsMap, vector<string>& requestWord, vector<string>& reqtWord,
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
	invertedIndex.wordSplitFunction(reqtWord, requestWord);
	reqtWord.clear();
	queryProcessingFunction(requestWord, searchResult, countWordsMap, docConfig, dataConfig, absoluteRelevance, maxAbsoluteRelevance, maxAbsoluteRelevance, getDataRequest, dataWord);
	searchRequestRelevanceFunction(getDataRequest, resultVectorConfig, iterator, dataWord, requestAbsoluteRelevance, maxRequestAbsoluteRelevance, searchRequestResult);
	resultVectorConfig.push_back({"End of query "});
	requestResultConfig.clear();
	requestNumberConfig.clear();
	searchResult.clear();
	++reqNumber;
}

void ConverterJSON::searchAnswerFunction(multimap<string, vector< Entry>>& countWordsMap, map <size_t, vector< string>>& getRequest)
{
		vector<string> requestWord, reqtWord;
		int reqNumber = 1;
		nlohmann::json dataConfig[MAX_RESPONS], docConfig, requestNumberConfig, requestResultConfig;
		vector< nlohmann::json> resultVectorConfig;
		for (auto iterator = getRequest.begin(); iterator != getRequest.end(); iterator++)
		{
			requestProcessingFunction(iterator, countWordsMap, requestWord, reqtWord, reqNumber, dataConfig, docConfig, requestNumberConfig, requestResultConfig, resultVectorConfig);
		}
		answersJSON.writingDataFileFunction(resultVectorConfig);
}

void ConverterJSON::cleaningJSONArrayFunction(nlohmann::json requestConfig[MAX_RESPONS])
{
	for (int i = 0; i < MAX_RESPONS; ++i)
	{
		requestConfig[i].clear();
	}
}