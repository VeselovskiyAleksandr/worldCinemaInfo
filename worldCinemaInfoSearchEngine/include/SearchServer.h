#pragma once
#include "ConverterJSON.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono> 

using namespace std;

class SearchServer
{
public: SearchServer() = default;
	  ConverterJSON converterJSON;
	  AnswersJSON answersJSON;
	  InvertedIndex invertedIndex;
	  mutex film_mutex;
	  condition_variable cond;

	  //функции потоков
	  void readingFromDatabaseFunction(queue<string>& q, mutex& mtx, condition_variable& cond, atomic<bool>& fileComplete);
	  void ProcessData(const string& line);

	  //функция открытия файлов с документами
	  void openFilesFunction(Configuration& configuration, string moviePath[DOCUMENT_NUMBER], ifstream file[DOCUMENT_NUMBER]);

	  //функция получения данных о словах
	  void getWordDataFunction(multimap<string, vector< Entry>>& countWordsMap, Configuration& configuration, vector<string> wordsFromFilesVector[DOCUMENT_NUMBER],
		                       vector<Entry> getCountWords[DOCUMENT_NUMBER]);

	  //функция чтения из файлов
	  void readFromFilesFunction(vector<string> wordsFromFilesVector[DOCUMENT_NUMBER], string strWord[DOCUMENT_NUMBER], ifstream file[DOCUMENT_NUMBER], queue<string> lines);

	  template<typename JsonIterator>
	  void conditionCheckingFunction(map<size_t, vector<string>>& getRequests, vector<string>& vecRequest, size_t& nReq, const JsonIterator& element);

	  template<typename JsonType>
	  void JsonTraversalFunction_1(map<size_t, vector<string>>& getRequests, vector<string>& vecRequest, size_t& nReq, const JsonType& data);

	  //функция получения запросов
	  void getRequestsFunction(map<size_t, vector<string>>& getRequests);

	  //функция заполнения вектора ответов
	  template<typename JsonType>
	  void fillingAnswerVectorFunction(JsonType& it, size_t& nReq, string& val, string& key, vector<string>& vecAnswer);

	  //функция обхода nlohmann::JSON 
	  template<typename JsonType>
	  void JsonTraversalFunction_2(const JsonType& data, size_t& nReq, string& val, string& key, vector<string>& vecAnswer);

	  //функция получения данных из файла
	  template<typename JsonType>
	  void extractDataFileFunction(JsonType& ansconfig, size_t& nReq, string& val, string& key, vector<string>& vecAnswer);

	  //функция вывода результатов поиска
	  void getAnswerFunction();
};
