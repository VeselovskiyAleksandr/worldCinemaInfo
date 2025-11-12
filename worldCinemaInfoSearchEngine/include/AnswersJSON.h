#pragma once
#include "InvertedIndex.h"

class AnswersJSON
{
public: AnswersJSON() = default;
	  InvertedIndex invertedIndex;

	  //функция заполняет структуру Entry в карте для каждого слова
	  void searchIdenticalWordsFunction(multimap<string, vector< Entry>>& countWordsMap, multimap<size_t, size_t>& searchResult,
		                                vector<string>& requestWord, size_t& absoluteRelevance, size_t& maxAbsoluteRelevance, size_t& maxAbsoluteRelevanceDoc, int& i,
		                                multimap < size_t, vector <size_t>> & getDataRequest, multimap < string, multimap <size_t, size_t>>& dataWord);

	  //функция записи содержимого в JSON в файл
	  void writingDataFileFunction(vector< nlohmann::json>& resultVectorConfig);

	  //функция вывода ответов
	   void responseOutputFunction(vector<string>& vecAnswer);
};