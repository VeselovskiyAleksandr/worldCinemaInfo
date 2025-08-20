#include "ConverterJSON.h"

void ConverterJSON::wordSplitFunction(vector<string>& sentence, vector<string>& setWords)
{
	string highlightedWord = "";
	for (int ir = 0; ir < sentence.size(); ++ir)
	{
		for (int n = 0; n < sentence[ir].length(); ++n)
		{
			if (sentence[ir][n] != '?' && sentence[ir][n] != '!' && sentence[ir][n] != '"' && sentence[ir][n] != '-' && sentence[ir][n] != '\'' && sentence[ir][n] != '/' && sentence[ir][n] != ')' && sentence[ir][n] != '(' && sentence[ir][n] != ' ' && sentence[ir][n] != ';' && sentence[ir][n] != ':' && sentence[ir][n] != ',' && sentence[ir][n] != '.')
			{
				if (sentence[ir][n] >= 65 && sentence[ir][n] <= 90)
				{
					sentence[ir][n] += 32;
				}
				highlightedWord += sentence[ir][n];
			}
			else
			{
				if (highlightedWord.length() > configuration.minWordLength)
					setWords.push_back(highlightedWord);
				highlightedWord = "";
			}
			if (ir == sentence[ir].length() - 1)
			{
				setWords.push_back(highlightedWord);
				highlightedWord = "";
			}
		}
	}
}

void ConverterJSON::vectorEntryFillFunction(vector<string> vectWord[DOCUMENT_NUMBER], int filmId, multimap<string, vector< Entry>>& countWordM)
{
	size_t wordRepetitionCount = 0;
	vector<string> checkRepetition;
	for (int itr = 0; itr < vectWord[filmId].size(); ++itr)
	{
		for (int iter = itr + 1; iter < vectWord[filmId].size(); ++iter)
		{
			if (vectWord[filmId][itr] == vectWord[filmId][iter])
			{
				wordRepetitionCount++;
			}
		}
		wordRepetitionCount++;
		auto pointer = find(checkRepetition.begin(), checkRepetition.end(), vectWord[filmId][itr]);
		if ((pointer == end(checkRepetition)) && (vectWord[filmId][itr].size() > configuration.minWordLength))
		{
			Entry entry;
			entry.docId = filmId;
			entry.freqWordsCount = wordRepetitionCount;
			entry.word = vectWord[filmId][itr];
			vector<Entry> getCountWords;
			getCountWords.push_back(entry);
			countWordM.insert({ entry.word, getCountWords });
			getCountWords.clear();
		}
		checkRepetition.push_back(vectWord[filmId][itr]);
		wordRepetitionCount = 0;
	}
	checkRepetition.clear();
}

void ConverterJSON::readingFromDatabase(queue<string>& q, mutex& mtx, condition_variable& cond, atomic<bool>& fileComplete)
{
	while (fileComplete)
	{
		unique_lock<mutex> lock(mtx);
		cond.wait(lock, [&] { return !q.empty() || !fileComplete; });
		if (q.empty())
		{
			continue;
		}
		string line = q.front();
		q.pop();
		lock.unlock();
		ProcessData(line);
	}
}

void ConverterJSON::ProcessData(const string& line)
{
	lock_guard< mutex > lk(film_mutex);
}

void ConverterJSON::readFromFilesFunction(vector<string> wordsFromFilesVector[DOCUMENT_NUMBER], Configuration& configuration)
{
		vector<string> vectorWord[DOCUMENT_NUMBER];
		string strWord[DOCUMENT_NUMBER], moviePath[DOCUMENT_NUMBER];
		ifstream file[DOCUMENT_NUMBER];
	    queue<string> lines;
	for (int i = 0; i < DOCUMENT_NUMBER; ++i)
	{
		moviePath[i] = configuration.movieTitles.at(i + 1);
		file[i].open(moviePath[i]);
		if (!file[i].is_open())
		{
			cerr << "\n" << "The file " << configuration.movieTitles.at(i + 1) << " is not found." << "\n";
		}
		file[i].seekg(0);
	}
	for (int i = 0; i < DOCUMENT_NUMBER; ++i)
	{
		string key = to_string(i);
		wordsFromFilesVector[i].push_back("\ndoc_id: ");
		wordsFromFilesVector[i].push_back(key);
		wordsFromFilesVector[i].push_back("\n ");
		key = "";
		while (getline(file[i], strWord[i]))
		{
			lines.push(strWord[i]);
			wordsFromFilesVector[i].push_back(strWord[i]);
			strWord[i] = "";
		}
		file[i].close();
	}
}

void ConverterJSON::getWordDataFunction(multimap<string, vector< Entry>>&countWordsMap, Configuration & configuration, vector<string> wordsFromFilesVector[DOCUMENT_NUMBER])
{
	vector<string>vectorWord[DOCUMENT_NUMBER];
	string strWord[DOCUMENT_NUMBER], moviePath[DOCUMENT_NUMBER];
	ifstream file[DOCUMENT_NUMBER];
	mutex mtx;
	queue<string> lines;
	atomic<bool> fileComplete = true;
	for (int i = 0; i < DOCUMENT_NUMBER; ++i)
	{
		string key = to_string(i);
		wordsFromFilesVector[i].push_back("\ndoc_id: ");
		wordsFromFilesVector[i].push_back(key);
		wordsFromFilesVector[i].push_back("\n ");
		key = "";
		while (getline(file[i], strWord[i]))
		{
			lines.push(strWord[i]);
			wordsFromFilesVector[i].push_back(strWord[i]);
			strWord[i] = "";
		}
		file[i].close();
		vector<thread> threads;
		for (int i = 0; i < configuration.numberProcessorCores; ++i)
		{
			threads.emplace_back(&ConverterJSON::readingFromDatabase, this, ref(lines), ref(mtx), ref(cond), ref(fileComplete));
		}
		wordSplitFunction(wordsFromFilesVector[i], vectorWord[i]);
		wordsFromFilesVector[i].clear();
		vectorEntryFillFunction(vectorWord, i, countWordsMap);
		fileComplete = false;
		cond.notify_all();
		for (auto& thread : threads)
		{
			thread.join();
		}
		for (int ip = 0; ip < DOCUMENT_NUMBER; ++ip)
		{
			vectorWord[i].clear();
		}
	}
}