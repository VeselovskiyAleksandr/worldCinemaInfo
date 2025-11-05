#pragma once
#include "EngineStart.h"

class RequestsJSON
{
public: RequestsJSON() = default;
public:
     Configuration configuration;
	 map <size_t, vector< string>> getRequests;

	 //функция проверки строкового типа для присвоения значения
	 template<typename JsonIterator>
	 void conditionCheckingFunction(map<size_t, vector<string>>& getRequests, vector<string>& vecRequest, size_t& nReq, const JsonIterator& element);

	 //функция обхода nlohmann::json
	 template<typename JsonType>
	 void JsonTraversalFunction(map<size_t, vector<string>>& getRequests, vector<string>& vecRequest, size_t& nReq, const JsonType& data);

	 //функция отбора слов превышающих минимальную длину
	 template<typename Iterator, typename vectorJSON>
	 void selectingWordsMinimumLength(const typename Iterator& iter, Configuration& configuration, vectorJSON& strConfig, size_t requestNumber);
	 
	 //функция обхода карты map
	 template<typename vectorJSON>
	 void mapTraversalFunction(map<size_t, vector<string>>& getRequests, vectorJSON& strConfig);

	   //функция получения запросов
	 void getRequestsFunction(map<size_t, vector<string>>& getRequests);

	  //функция ввода запроса
	  void requerInputFunction(map<size_t, vector<string>>& getRequests);

	  //функция записи запросов в файл
	  void writeMapToFileFunction(map<size_t, vector<string>>& getRequests);
};