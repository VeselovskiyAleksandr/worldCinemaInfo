#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include "Configuration.h"
#include "Entry.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "nlohmann/json.hpp"
#include <filesystem>
#include <chrono> 

using namespace std;
using json = nlohmann::json;

class ConverterJSON
{
public: ConverterJSON() = default;
	  Configuration configuration;
	  map <int, vector< string>> getRequests;
	  multimap<string, vector< Entry>> countWordsMap;
	  mutex film_mutex;
	  condition_variable cond;

	  //функция начала работы движка
	  void start();

	  //функция проверки открытия файла
	  void openFile(string path);

	  //функция получения запросов
	  void getRequestsFunction(map<int, vector<string>>& getRequests);

	  //функция ввода запроса
	  void requerInputFunction(map<int, vector<string>>& getRequests);

	  //функция выделения слов
	  void wordSplitFunction(vector<string>& sentence, vector<string>& setWords);

	  //функция записи данных в карту
	  void multiMapFillFunction(vector<Entry>& vectEntr, vector<Entry>& getWC, multimap<string, vector< Entry>>& countWM);

	  //функция заполнения вектора Entry
	  void vectorEntryFillFunction(vector<string> vectWord[DOCUMENT_NUMBER], vector<Entry> getWC[DOCUMENT_NUMBER], int n);

	  //функции потоков
	  void readingFromDatabase(queue<string>& q, mutex& mtx, condition_variable& cond, atomic<bool>& fileComplete);
	  void ProcessData(const string& line);

	  //функция подсчёта слов в документе
	  void wordCountFunction(multimap<string, vector< Entry>>& countWordsMap);

	  //функция поиска ответов. Здесь ведётся поиск в контейнере.
	  void searchAnswerFunction(multimap<string, vector< Entry>>& countWordsMap, map <int, vector< string>> getRequest);

	  //функция вывода результатов поиска
	  void getAnswerFunction();
};