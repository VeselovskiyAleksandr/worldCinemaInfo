#pragma once
#include "EngineStart.h"
#include "Entry.h"
#include "ConverterJSON.h"
#include "RequestsJSON.h"

class AnswersJSON
{
public: AnswersJSON() = default;
	  ConverterJSON converterJSON;
	  RequestsJSON requestsJSON;
	  Entry entry;

	  //функция заполняет структуру Entry в карте для каждого слова
	  void searchIdenticalWordsFunction(multimap<string, vector< Entry>>& countWordsMap, multimap<size_t, size_t>& searchResult,
		                                vector<string>& requestWord, size_t& absoluteRelevance, size_t& maxAbsoluteRelevance, size_t& maxAbsoluteRelevanceDoc, int& i,
		                               multimap < size_t, vector <size_t>> & getDataRequest, multimap < string, multimap <size_t, size_t>>& dataWord);

	  //функция заполнения вектора строк словами запросов
	  template<typename JsonIterator>
	  void writingQueriesVectorFunction(vector<string>& reqtWord, JsonIterator& iterator);

	  //функция вычисления относительной релевантности и запись её в массив nlohmann::json
	  void relevanceRecordFunction(nlohmann::json dataConfig[MAX_RESPONS], nlohmann::json& requestResultConfig, double& relativeRelevance, size_t& maxAbsoluteRelevance);

	  template<typename Iterat, typename Iter>
	  void vectorTraversalFunction(vector <size_t>& docIDVector1, vector <size_t>& docIDVector2, bool ifDoc, nlohmann::json& requestNumberConfig,
		                           nlohmann::json requestConfig[MAX_RESPONS], int& countReqResponses, Iterat& iterator, Iter& iter);

	  template<typename Iterator>
	  void nullResultRecordingFunction(vector <size_t>& docIDVector_1, vector <size_t>& docIDVector_2, nlohmann::json& requestNumberConfig,
		                               nlohmann::json requestConfig[MAX_RESPONS], Iterator& iterator, int& countReqResponses, nlohmann::json& docConfig);

	  void transferElementsFunction(vector <size_t>& docIDVector, vector <size_t>& dIDVector);

	  template<typename Iterator>
	  void contentLeastFrequentWordFunction(Iterator& iter, multimap < size_t, vector <size_t>>& getDataRequest, vector <size_t>& docIDVector);

	  void findingRequestDataFunction(multimap <size_t, size_t >& searchRequestResult, double& relativeReqRelevance, size_t& maxRequestAbsoluteRelevance,
		                              nlohmann::json requestConfig[MAX_RESPONS], int& countReqResponses, bool& ifDoc, nlohmann::json requestNumberConfig);

	  void cleaningJSONArrayFunction(nlohmann::json requestConfig[MAX_RESPONS]);

	  void relevanceIncreasingFunction(vector <size_t> dIDVector, multimap < string, multimap <size_t, size_t >>& dataWord, int& i, size_t& requestAbsoluteRelevance);

	  //функция обработки запроса
	  template<typename Iterator>
	  void requestProcessingFunction(Iterator& iterator, multimap<string, vector< Entry>>& countWordsMap, vector<string>& requestWord, vector<string>& reqtWord,  int& reqNumber,
		                             nlohmann::json dataConfig[MAX_RESPONS], nlohmann::json docConfig, nlohmann::json requestNumberConfig, nlohmann::json requestResultConfig,
		                             vector< nlohmann::json>& resultVectorConfig);

	  void writingDataFileFunction(vector< nlohmann::json>& resultVectorConfig);

	  template<typename Iterator>
	  void findingDocumentsRequestFunction(multimap < size_t, vector <size_t>>& getDataRequest, vector <size_t>& docIDVector, vector <size_t>& dIDVector, nlohmann::json requestConfig[MAX_RESPONS],
		                                   nlohmann::json& requestNumberConfig, bool& ifDoc, int& countReqResponses, Iterator& iterator);

	  void AnswersJSON::maxRelevanceFindingFunction(vector <size_t> dIDVector, multimap < string, multimap <size_t, size_t >>& dataWord, size_t& requestAbsoluteRelevance,
		                                            size_t& maxRequestAbsoluteRelevance, multimap <size_t, size_t >& searchRequestResult);


	  template<typename Iterator>
	  void searchRequestRelevanceFunction(multimap < size_t, vector <size_t>>& getDataRequest, vector< nlohmann::json>& resultVectorConfig, Iterator& iterator,
		                                  multimap < string, multimap <size_t, size_t>>& dataWord, size_t& requestAbsoluteRelevance, size_t& maxRequestAbsoluteRelevance,
		                                  multimap <size_t, size_t >& searchRequestResult);

	  //функция обработки запросов
	  void queryProcessingFunction(vector<string>& requestWord, multimap<size_t, size_t>& searchResult, multimap<string, vector< Entry>>& countWordsMap,
		                           nlohmann::json& docConfig, nlohmann::json dataConfig[MAX_RESPONS], size_t& absoluteRelevance, size_t& maxAbsoluteRelevance,
		                           size_t& maxAbsoluteRelevanceDoc, multimap < size_t,vector <size_t>> & getDataRequest, multimap < string, multimap <size_t, size_t>>& dataWord);

	  //функция поиска ответов. Здесь ведётся поиск в контейнере.
	  void searchAnswerFunction(multimap<string, vector< Entry>>& countWordsMap, map <size_t, vector< string>> getRequest);

	  //функция обхода nlohmann::JSON 
	  template<typename JsonType>
	  void JsonTraversalFunction(const JsonType& data, int nReq, string val, string key, vector<string>& vecAnswer);

	  //функция заполнения вектора ответов
	  template<typename JsonType>
	  void fillingAnswerVector(JsonType& it, int nReq, string val, string key, vector<string>& vecAnswer);

	  //функция вывода ответов
	  void responseOutputFunction(vector<string>& vecAnswer);

	  //функция получения данных из файла
	  template<typename JsonType>
	  void extractDataFileFunction(JsonType& ansconfig, int nReq, string val, string key, vector<string>& vecAnswer);

	  //функция вывода результатов поиска
	  void getAnswerFunction();
};