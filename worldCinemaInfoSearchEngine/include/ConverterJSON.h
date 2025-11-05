#pragma once
#include "EngineStart.h"
#include "Configuration.h"
#include "Entry.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono> 

class ConverterJSON
{
public: ConverterJSON() = default;
	  Configuration configuration;
	  vector<string> wordsFromFilesVector[DOCUMENT_NUMBER];
	  vector<Entry> getCountWords[DOCUMENT_NUMBER];
	  multimap<string, vector< Entry>> countWordsMap;
	  mutex film_mutex;
	  condition_variable cond;

	  //функция обхода вектора
	  void vectorTraversalFunction(vector<string>& sentence, vector<string>& setWords, string word);

	  //функция проверки условий для записи слова в поиск
	  template<typename Iterator>
	  void wordEntryConditionFunction(string& word, vector<string>& sentence, vector<string>& setWords, Iterator& ir, Iterator& n);

	  //функция выделения слов
	  void wordSplitFunction(vector<string>& sentence, vector<string>& setWords);

	  //функция заполнения вектора Entry. Содержит структуру Entry для каждого слова. 
	  void vectorEntryFillFunction(vector<string> vectWord[DOCUMENT_NUMBER], int n, vector<Entry> getCountWords[DOCUMENT_NUMBER]);

	  //функции потоков
	  void readingFromDatabase(queue<string>& q, mutex& mtx, condition_variable& cond, atomic<bool>& fileComplete);
	  void ProcessData(const string& line);

	  void openFilesFunction(Configuration& configuration, string moviePath[DOCUMENT_NUMBER], ifstream file[DOCUMENT_NUMBER]);

	  //функция чтения из файлов
	  void readFromFilesFunction(vector<string> wordsFromFilesVector[DOCUMENT_NUMBER], string strWord[DOCUMENT_NUMBER], ifstream file[DOCUMENT_NUMBER], queue<string> lines);

	  template<typename Iterator>
	  void fillEntryFunction(size_t countPosition, int& i, vector<Entry> getCountWords[DOCUMENT_NUMBER], Iterator& it, size_t& count, Entry& entry);

	  template<typename Iterator>
	  void matchSearchInnerLoopFunction(int& i, vector<Entry> getCountWords[DOCUMENT_NUMBER], Iterator& it, size_t& count);

	  void matchSearchOuterLoopFunction(vector<Entry> getCountWords[DOCUMENT_NUMBER], vector <Entry>& vectEntry, multimap<string, vector< Entry>>& countWordsMap);

	  //функция получения данных о словах
	  void getWordDataFunction(multimap<string, vector< Entry>>& countWordsMap, Configuration& configuration, vector<string> wordsFromFilesVector[DOCUMENT_NUMBER],
		                       vector<Entry> getCountWords[DOCUMENT_NUMBER]);
};