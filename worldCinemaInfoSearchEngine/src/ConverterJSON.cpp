#include "ConverterJSON.h"

void ConverterJSON::vectorTraversalFunction(vector<string>& sentence, vector<string>& setWords, string word)
{
	for (int ir = 0; ir < sentence.size(); ++ir)
	{
		for (int n = 0; n < sentence[ir].length(); ++n)
		{
			wordEntryConditionFunction(word, sentence, setWords, ir, n);
		}
	}

}

template<typename Iterator>
void ConverterJSON::wordEntryConditionFunction(string& word, vector<string>& sentence, vector<string>& setWords, Iterator& ir, Iterator& n)
{
	if (sentence[ir][n] != '?' && sentence[ir][n] != '!' && sentence[ir][n] != '"' && sentence[ir][n] != '-' && sentence[ir][n] != '\'' && sentence[ir][n] != '/' && sentence[ir][n] != ')' && sentence[ir][n] != '(' && sentence[ir][n] != ' ' && sentence[ir][n] != ';' && sentence[ir][n] != ':' && sentence[ir][n] != ',' && sentence[ir][n] != '.')
	{
		if (sentence[ir][n] >= 65 && sentence[ir][n] <= 90)
		{
			sentence[ir][n] += 32;
		}
		word += sentence[ir][n];
	}
	else
	{
		if (word.length() > configuration.minWordLength)
			setWords.push_back(word);

		word = "";
	}
	if (ir == sentence[ir].length() - 1)
	{
		setWords.push_back(word);
		word = "";
	}
}

void ConverterJSON::wordSplitFunction(vector<string>& sentence, vector<string>& setWords)
{
	string highlightedWord = "";
	vectorTraversalFunction(sentence, setWords, highlightedWord);
}

void ConverterJSON::vectorEntryFillFunction(vector<string> vectWord[DOCUMENT_NUMBER], int filmId, vector<Entry> getCountWords[DOCUMENT_NUMBER])
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
		Entry entry;
		auto pointer = find(checkRepetition.begin(), checkRepetition.end(), vectWord[filmId][itr]);
		if ((pointer == end(checkRepetition)) && (vectWord[filmId][itr].size() > configuration.minWordLength))
		{
			entry.freqWordsCount = wordRepetitionCount;
			entry.docId = filmId;
			entry.word = vectWord[filmId][itr];
			entry.checkAction = false;
			getCountWords[filmId].push_back(entry);
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

void ConverterJSON::openFilesFunction(Configuration& configuration, string moviePath[DOCUMENT_NUMBER], ifstream file[DOCUMENT_NUMBER])
{
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
}

void ConverterJSON::readFromFilesFunction(vector<string> wordsFromFilesVector[DOCUMENT_NUMBER], string strWord[DOCUMENT_NUMBER], ifstream file[DOCUMENT_NUMBER], queue<string> lines)
{
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

template<typename Iterator>
void ConverterJSON::fillEntryFunction(size_t countPosition, int& i, vector<Entry> getCountWords[DOCUMENT_NUMBER], Iterator& it, size_t& count, Entry& entry)
{
	entry.docId = it.docId;
	entry.word = it.word;
	entry.wordFrequency = count;
	entry.checkAction = true;
	getCountWords[i][countPosition] = entry;
}

template<typename Iterator>
void ConverterJSON::matchSearchInnerLoopFunction(int& i, vector<Entry> getCountWords[DOCUMENT_NUMBER], Iterator& it, size_t& count)
{
	for (int j = i + 1; j != DOCUMENT_NUMBER; ++j)
	{
		size_t countPos_2 = 0;
		for (auto& qj : getCountWords[j])
		{
			if ((it.word == qj.word) && (qj.checkAction != true))
			{
				count += qj.freqWordsCount;

				Entry entry;
				fillEntryFunction(countPos_2, i, getCountWords, it, count, entry);
				break;
			}
			countPos_2++;
		}
	}
}

void ConverterJSON::matchSearchOuterLoopFunction(vector<Entry> getCountWords[DOCUMENT_NUMBER], vector <Entry>& vectEntry, multimap<string, vector< Entry>>& countWordsMap)
{
	for (int i = 0; i != DOCUMENT_NUMBER; ++i)
	{
		size_t countPos_1 = 0;
		for (auto& it : getCountWords[i])
		{
			size_t count = it.freqWordsCount;
			if (it.checkAction == true)
				break;
			matchSearchInnerLoopFunction(i, getCountWords, it, count);
			Entry entry;
			entry.freqWordsCount = it.freqWordsCount;
			fillEntryFunction(countPos_1, i, getCountWords, it, count, entry);
			vectEntry.push_back(entry);
			countWordsMap.insert({ it.word, vectEntry });
			vectEntry.clear();
			count = 0;
			++countPos_1;
		}
	}
}

void ConverterJSON::getWordDataFunction(multimap<string, vector< Entry>>& countWordsMap, Configuration& configuration, vector<string> wordsFromFilesVector[DOCUMENT_NUMBER],
	                                    vector<Entry> getCountWords[DOCUMENT_NUMBER])
{
	vector<string>vectorWord[DOCUMENT_NUMBER];
	string strWord[DOCUMENT_NUMBER], moviePath[DOCUMENT_NUMBER];
	ifstream file[DOCUMENT_NUMBER];
	mutex mtx;
	queue<string> lines;
	atomic<bool> fileComplete = true;
	openFilesFunction(configuration, moviePath, file);
	readFromFilesFunction(wordsFromFilesVector, strWord, file,  lines);
	for (int i = 0; i < DOCUMENT_NUMBER; ++i)
	{
		vector<thread> threads;
		for (int i = 0; i < configuration.numberProcessorCores; ++i)
		{
			threads.emplace_back(&ConverterJSON::readingFromDatabase, this, ref(lines), ref(mtx), ref(cond), ref(fileComplete));
		}
		wordSplitFunction(wordsFromFilesVector[i], vectorWord[i]);
		wordsFromFilesVector[i].clear();
		vectorEntryFillFunction(vectorWord, i, getCountWords);
		fileComplete = false;
		cond.notify_all();
		for (auto& thread : threads)
		{
			thread.join();
		}
		vector <Entry> vectEntry;
		matchSearchOuterLoopFunction(getCountWords, vectEntry, countWordsMap);
			vectorWord[i].clear();
			getCountWords[i].clear(); 
	}
}