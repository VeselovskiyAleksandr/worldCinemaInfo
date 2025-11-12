#pragma once
#include <string>

struct Entry
{
	size_t docId, freqWordsCount; //номер документа и повторяемость слова в документе
	size_t wordFrequency; //повторяемость слова во всех документах
	std::string word; //содержит слово
	bool checkAction; //предотвращает подсчёт слова на одной позиции несколько раз
};