#pragma once
#include "EngineStart.h"

class RequestsJSON
{
public: RequestsJSON() = default;
public:
     Configuration configuration;
	 map <int, vector< string>> getRequests;

	 template<typename JsonIterator>
	 void conditionCheckingFunction(
		 map<int, vector<string>>& getRequests,
		 vector<string>& vecRequest,
		 int& nReq,
		 const typename JsonIterator& element);

	 template<typename JsonType>
	 void vectorTraversalFunction(
		 map<int, vector<string>>& getRequests,
		 vector<string>& vecRequest,
		 int& nReq, const JsonType& data);

	 template<typename Iterator>
	 void selectingWordsMinimumLength(string requerie, const typename Iterator& iter, Configuration& configuration);
	 
	 template<typename JsonIterator, typename VectorJSON>
	 void vectorTraversalFunction2(
		 map<int, vector<string>>& getRequests,
		 vector<string>& vecRequest,string requerie, 
		 size_t& nReq, const typename JsonIterator& element,
		  VectorJSON& strConfig);

	   //функция получения запросов
	 void getRequestsFunction(map<int, vector<string>>& getRequests);

	  //функция ввода запроса
	  void requerInputFunction(map<int, vector<string>>& getRequests);
};