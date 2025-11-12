#include "SearchServer.h"

void SearchServer::readingFromDatabaseFunction(queue<string>& q, mutex& mtx, condition_variable& cond, atomic<bool>& fileComplete)
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

void SearchServer::ProcessData(const string& line)
{
	lock_guard< mutex > lk(film_mutex);
}

void SearchServer::openFilesFunction(Configuration& configuration, string moviePath[DOCUMENT_NUMBER], ifstream file[DOCUMENT_NUMBER])
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

void SearchServer::getWordDataFunction(multimap<string, vector< Entry>>& countWordsMap, Configuration& configuration, vector<string> wordsFromFilesVector[DOCUMENT_NUMBER],
	vector<Entry> getCountWords[DOCUMENT_NUMBER])
{
	vector<string>vectorWord[DOCUMENT_NUMBER];
	string strWord[DOCUMENT_NUMBER], moviePath[DOCUMENT_NUMBER];
	ifstream file[DOCUMENT_NUMBER];
	mutex mtx;
	queue<string> lines;
	atomic<bool> fileComplete = true;
	openFilesFunction(configuration, moviePath, file);
	readFromFilesFunction(wordsFromFilesVector, strWord, file, lines);
	for (int i = 0; i < DOCUMENT_NUMBER; ++i)
	{
		vector<thread> threads;
		for (int i = 0; i < configuration.numberProcessorCores; ++i)
		{
			threads.emplace_back(&SearchServer::readingFromDatabaseFunction, this, ref(lines), ref(mtx), ref(cond), ref(fileComplete));
		}
		invertedIndex.wordSplitFunction(wordsFromFilesVector[i], vectorWord[i]);
		wordsFromFilesVector[i].clear();
		invertedIndex.vectorEntryFillFunction(vectorWord, i, getCountWords);
		fileComplete = false;
		cond.notify_all();
		for (auto& thread : threads)
		{
			thread.join();
		}
		vector <Entry> vectEntry;
		invertedIndex.matchSearchOuterLoopFunction(getCountWords, vectEntry, countWordsMap);
		vectorWord[i].clear();
		getCountWords[i].clear();
	}
}

void SearchServer::readFromFilesFunction(vector<string> wordsFromFilesVector[DOCUMENT_NUMBER], string strWord[DOCUMENT_NUMBER], ifstream file[DOCUMENT_NUMBER], queue<string> lines)
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

template <typename JsonIterator>
void SearchServer::conditionCheckingFunction(map <size_t, vector<string>>& getRequests, vector<string>& vecRequest, size_t& nReq, const JsonIterator& itr)
{
	string strValue = itr.value().is_string() ? itr.value().get<string>() : to_string(itr.value());
	if (!strValue.empty() && strValue != "Req")
	{
		strValue = strValue.substr(4, strValue.size() - 6);
		vecRequest.push_back(strValue);
		getRequests.emplace(nReq, vecRequest);
		nReq++;
	}
	vecRequest.clear();
}

template<typename JsonType>
void SearchServer::JsonTraversalFunction_1(map<size_t, vector<string>>& getRequests, vector<string>& vecRequest, size_t& nReq, const JsonType& data)
{
	for (const auto& item : data.items())
	{
		for (const auto& itr : item.value().items())
		{
			conditionCheckingFunction(getRequests, vecRequest, nReq, itr);
		}
	}
}

void SearchServer::getRequestsFunction(map<size_t, vector <string>>& getRequests)
{
	ifstream reqfile("requests.json");
	if (!reqfile)
		cout << "\n " << "Requests file is not found.";
	if (reqfile.peek() != EOF)
	{
		nlohmann::json reqconfig;
		reqfile.seekg(0);
		reqfile >> reqconfig;
		reqfile.close();
		size_t nReq = 0;
		vector<string> vecRequest;
		JsonTraversalFunction_1(getRequests, vecRequest, nReq, reqconfig);
	}
}

template<typename JsonType>
 void SearchServer::fillingAnswerVectorFunction(JsonType& it, size_t& nReq, string& val, string& key, vector<string>& vecAnswer)
{
	for (const auto& im : it.value().items())
	{
			val = to_string(im.value());
			vecAnswer.push_back(val);
			val = "";
	}
}

template<typename JsonType>
 void SearchServer::JsonTraversalFunction_2(const JsonType& ir, size_t& nReq, string& val, string& key, vector<string>& vecAnswer)
{
	for (const auto& iq : ir.value().items())
	{
		for (const auto& it : iq.value().items())
		{
			fillingAnswerVectorFunction(it, nReq, val, key, vecAnswer);
		}
	}
}

template<typename JsonType>
 void SearchServer::extractDataFileFunction(JsonType& ansconfig, size_t& nReq, string& val, string& key, vector<string>& vecAnswer)
{
	for (const auto& item : ansconfig.items())
	{
		for (const auto& ir : item.value().items())
		{
			JsonTraversalFunction_2(ir, nReq, val, key, vecAnswer);
		}
	}
}

void SearchServer::getAnswerFunction()
{
	ifstream ansfile("answers.json");
	if (!ansfile)
		cout << "\n " << "Requests file is not found.";
	if (ansfile.peek() != EOF)
	{
		nlohmann::json ansconfig;
		size_t nReq = 0;
		string val = "", key = "";
		vector<string> vecAnswer;
		if (ansfile.peek() != EOF)
		{
			ansfile.seekg(0);
			ansfile >> ansconfig;
			ansfile.close();
			extractDataFileFunction(ansconfig, nReq, val, key, vecAnswer);
		}
		answersJSON.responseOutputFunction(vecAnswer);
	}
}