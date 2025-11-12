#pragma once
#include "Configuration.h"
#include "Entry.h"
#include "RequestsJSON.h"

class InvertedIndex
{
	public: InvertedIndex() = default;
	  Configuration configuration;
	  Entry entry;
	  RequestsJSON requestsJSON;
	  vector<string> wordsFromFilesVector[DOCUMENT_NUMBER];
	  vector<Entry> getCountWords[DOCUMENT_NUMBER];
	  multimap<string, vector< Entry>> countWordsMap;

	  	  //функция обхода вектора
			void vectorTraversalFunction(vector<string>& sentence, vector<string>& setWords, string word);

			//функция проверки условий для записи слова в поиск
			template<typename Iterator>
			void wordEntryConditionFunction(string& word, vector<string>& sentence, vector<string>& setWords, Iterator& ir, Iterator& n);
			
			//функция выделения слов
			void wordSplitFunction(vector<string>& sentence, vector<string>& setWords);

			//функция заполнения вектора Entry. Содержит структуру Entry для каждого слова.
			void vectorEntryFillFunction(vector<string> vectWord[DOCUMENT_NUMBER], int n, vector<Entry> getCountWords[DOCUMENT_NUMBER]);

			//функция поиска документов, в которых встречается слово с расчётом абсолютной релевантности
			void relevanceIncreasingFunction(vector <size_t> dIDVector, multimap < string, multimap <size_t, size_t >>& dataWord, int& i, size_t& requestAbsoluteRelevance);

			//функция наождения максимального релеванса
			void maxRelevanceFindingFunction(vector <size_t> dIDVector, multimap < string, multimap <size_t, size_t >>& dataWord, size_t& requestAbsoluteRelevance,
				                             size_t& maxRequestAbsoluteRelevance, multimap <size_t, size_t >& searchRequestResult);

			//функция заполнения структуры Entry
			template<typename Iterator>
			void fillEntryFunction(size_t& countPosition, int& i, vector<Entry> getCountWords[DOCUMENT_NUMBER], Iterator& it, size_t& count, Entry& entry);

			//функция подсчёта повторяемости слова (внутренни цикл)
			template <typename Iterator>
			void matchSearchInnerLoopFunction(int& i, vector<Entry> getCountWords[DOCUMENT_NUMBER], Iterator& it, size_t& count);

			//функция подсчёта повторяемости слова (внешний цикл)
			void matchSearchOuterLoopFunction(vector<Entry> getCountWords[DOCUMENT_NUMBER], vector <Entry>& vectEntry, multimap<string, vector< Entry>>& countWordsMap);

			//функция записи оставшихся элементов при нахождении общих документов для слов с разной повторяемостью из одного вектора в другой
			void transferElementsFunction(vector <size_t>& docIDVector, vector <size_t>& dIDVector);
	  };