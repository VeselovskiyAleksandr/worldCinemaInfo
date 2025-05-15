#pragma once

#include <iostream>
#include <string>
#include <map>

using namespace std;

#define MAX_SIZE 10000 //наибольшее общее количество слов в документах
#define MAX_RESPONS 5 //наибольщее число ответов
#define DOCUMENT_NUMBER 10 //количество документов

struct Configuration
{
	string name;//Содержит название движка
	string version;//версия движка
	map<int, string> movieTitles;
	static const int maxRequest = 1000;//максимальное количество зпросов
	static const int maxStrRequestLength = 80; //наибольшее количество знаков в запросе
};