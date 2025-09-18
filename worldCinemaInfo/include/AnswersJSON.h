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
		  vector<string>& requestWord, size_t& absoluteRelevance, size_t &maxAbsoluteRelevance, size_t& maxAbsoluteRelevanceDoc, int& i);

	  //функция записи в nlohmann::json номера документа и количества повторений
	  void recordNumberRepetitionsFunction(multimap<size_t, size_t>& searchResult, int& countResponses, nlohmann::json& docConfig);

	  //функция заполнения вектора строк словами запросов
	  template<typename JsonIterator>
	  void writingQueriesVectorFunction(vector<string>& reqtWord, JsonIterator& iterator);

	  //функция вычисления относительной релевантности и запись её в массив nlohmann::json
	  void relevanceRecordFunction(nlohmann::json dataConfig[MAX_RESPONS], nlohmann::json& requestResultConfig,
		  double& relativeRelevance, size_t& maxAbsoluteRelevance);

	  //функция заполнения массива nlohmann::json данными
	  void fillingDataJSONArrayFunction(nlohmann::json dataConfig[MAX_RESPONS], vector<string>& requestWord,
		  nlohmann::json docConfig, size_t absoluteRelevance, int i);

	  //функция обработки запросов
	  void queryProcessingFunction(vector<string> &requestWord, multimap<size_t, size_t>& searchResult,
		  multimap<string, vector< Entry>>& countWordsMap, nlohmann::json& docConfig, nlohmann::json dataConfig[MAX_RESPONS], size_t& absoluteRelevance,
		  size_t& maxAbsoluteRelevance, size_t& maxAbsoluteRelevanceDoc);

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