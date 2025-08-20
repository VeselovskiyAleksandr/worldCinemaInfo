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

	  //функция поиска ответов. Здесь ведётся поиск в контейнере.
	  void searchAnswerFunction(multimap<string, vector< Entry>>& countWordsMap, map <int, vector< string>> getRequest);

	  //функция вывода результатов поиска
	  void getAnswerFunction();
};