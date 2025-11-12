#pragma once
#include "EngineStart.h"

using namespace std;

class RequestsJSON
{
public: RequestsJSON() = default;
public:
     Configuration configuration;
	 map <size_t, vector< string>> getRequests;

	 //функция отбора слов превышающих минимальную длину
	 template<typename Iterator, typename vectorJSON>
	 void selectingWordsMinimumLength(const typename Iterator& iter, Configuration& configuration, vectorJSON& strConfig, size_t& requestNumber);
	 
	 //функция обхода карты map
	 template<typename vectorJSON>
	 void mapTraversalFunction(map<size_t, vector<string>>& getRequests, vectorJSON& strConfig);

	  //функция ввода запроса
	  void requerInputFunction(map<size_t, vector<string>>& getRequests);

	  //функция записи запросов в файл
	  void writeMapToFileFunction(map<size_t, vector<string>>& getRequests);
};