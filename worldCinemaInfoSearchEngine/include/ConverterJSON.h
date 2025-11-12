#pragma once
#include "AnswersJSON.h"
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class ConverterJSON
{
public: ConverterJSON() = default;
	  InvertedIndex invertedIndex;
	  AnswersJSON answersJSON;

	//функция записи в JSON нулевого результата
	template<typename Iterator>
	void nullResultRecordingFunction(vector <size_t>& docIDVector_1, vector <size_t>& docIDVector_2, nlohmann::json& requestNumberConfig,
		                             nlohmann::json requestConfig[MAX_RESPONS], Iterator& iterator, int& countReqResponses, nlohmann::json& docConfig);

	//функция нахождения общих документов при переходе от слов с меньшей встречаемостью к словам с большей встречаемостью.
template<typename Iterat, typename Iter>
void vectorTraversalFunction(vector <size_t>& docIDVector1, vector <size_t>& docIDVector2, bool ifDoc, nlohmann::json& requestNumberConfig,
	                         nlohmann::json requestConfig[MAX_RESPONS], int& countReqResponses, Iterat& iterator, Iter& iter);

//функция записи в вектор документов для наименее повторяющегося слова
template<typename Iterator>
void contentLeastFrequentWordFunction(Iterator& iter, multimap < size_t, vector <size_t>>& getDataRequest, vector <size_t>& docIDVector);

//функция расчёта релевантности и записи данных в JSON
void findingRequestDataFunction(multimap <size_t, size_t >& searchRequestResult, double& relativeReqRelevance, size_t& maxRequestAbsoluteRelevance,
                            	nlohmann::json requestConfig[MAX_RESPONS], int& countReqResponses, bool& ifDoc, nlohmann::json requestNumberConfig);

//функция очистки вектора JSON
void cleaningJSONArrayFunction(nlohmann::json requestConfig[MAX_RESPONS]);

//функция нахождения документов для всех слов из запроса
template<typename Iterator>
void findingDocumentsRequestFunction(multimap < size_t, vector <size_t>>& getDataRequest, vector <size_t>& docIDVector, vector <size_t>& dIDVector, nlohmann::json requestConfig[MAX_RESPONS],
	                                 nlohmann::json& requestNumberConfig, bool& ifDoc, int& countReqResponses, Iterator& iterator);

//функция получения результатов на запрос
template<typename Iterator>
void searchRequestRelevanceFunction(multimap < size_t, vector <size_t>>& getDataRequest, vector< nlohmann::json>& resultVectorConfig, Iterator& iterator,
	                                multimap < string, multimap <size_t, size_t>>& dataWord, size_t& requestAbsoluteRelevance, size_t& maxRequestAbsoluteRelevance,
	                                multimap <size_t, size_t >& searchRequestResult);

//функция обработки слов из запросов
void queryProcessingFunction(vector<string>& requestWord, multimap<size_t, size_t>& searchResult, multimap<string, vector< Entry>>& countWordsMap,
	                         nlohmann::json& docConfig, nlohmann::json dataConfig[MAX_RESPONS], size_t& absoluteRelevance, size_t& maxAbsoluteRelevance,
	                         size_t& maxAbsoluteRelevanceDoc, multimap < size_t, vector <size_t>>& getDataRequest, multimap < string, multimap <size_t, size_t>>& dataWord);

//функция заполнения вектора строк словами запросов
template<typename JsonIterator>
void writingQueriesVectorFunction(vector<string>& reqtWord, JsonIterator& iterator);

//функция обработки запросов
template<typename Iterator>
void requestProcessingFunction(Iterator& iterator, multimap<string, vector< Entry>>& countWordsMap, vector<string>& requestWord, vector<string>& reqtWord, int& reqNumber,
	                           nlohmann::json dataConfig[MAX_RESPONS], nlohmann::json docConfig, nlohmann::json requestNumberConfig, nlohmann::json requestResultConfig,
	                           vector< nlohmann::json>& resultVectorConfig);

//функция поиска ответов. Здесь ведётся поиск в контейнере.
void searchAnswerFunction( multimap<string, vector< Entry>>& countWordsMap, map <size_t, vector< string>>& getRequest);
};

