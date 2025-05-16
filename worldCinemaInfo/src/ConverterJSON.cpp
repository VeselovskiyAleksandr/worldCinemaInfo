#include "ConverterJSON.h"

void ConverterJSON::start()
{
	cout << "                              Launching a search engine... " << "\n";
	nlohmann::json config;
	ifstream cnfile("config.json");
	if (!cnfile)
	{
		throw exception();
	}
	cnfile >> config;
	cnfile.close();
	config["config"]["maxResponses"] = { MAX_RESPONS };
	cout << config["config"] << "\n";
	for (const auto& item : config["config"].items())
	{
		if (item.key() == "name")
		{
			configuration.name = item.value();
		}
		else if (item.key() == "version")
		{
			configuration.version = item.value();
		}
	}
	int count = 0;
	for (const auto& item : config["files"].items())
	{
		int id = stoi(item.key());
		pair<int, string> oPair(id, item.value());
		configuration.movieTitles.insert(oPair);
		count++;
	}
};

void ConverterJSON::openFile(string path)
{
	if (filesystem::exists(path))
	{
		if (filesystem::current_path() == "answers.json")
		{
			ofstream file("answers.json", ios::out | ios::trunc);
			file.close();
		}
		else
		{
			ofstream file;
			file.close();
		}
	}
	else
	{
		ofstream reqFile(path);
		reqFile.close();
	}
};

void ConverterJSON::getRequestsFunction(map<int, vector<string>>& getRequests)
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
		int nReq = 0;
		string val = "";
		vector<string> vecRequest;
		for (const auto& item : reqconfig.items())
		{
			for (const auto& ir : item.value().items())
			{
				for (const auto& iq : ir.value().items())
				{
					val = to_string(iq.value());
					if ((val[0] == '"') && (val != "Req") && (val.size() > 3))
					{
						val.erase(0, 1);
						if (val.size() > 4)
						{
							val.erase(val.size() - 1, 1);
							vecRequest.push_back(val);
							val = "";
							getRequests.emplace(nReq, vecRequest);
							vecRequest.clear();
							nReq++;
						}
					}
				}
			}
		}
	}
}

void ConverterJSON::requerInputFunction(map<int, vector<string>>& getRequests)
{
	if (getRequests.size() < configuration.maxRequest)
	{
		cout << "\n" << "                                      Information about domestic and foreign films" << "\n" << "\n";
		cout << "               Search query field" << "\n" << "\n";
		vector<string> vectorRequest;
		string requerie = "";
		int requestNumber = 1;
		getline(cin, requerie);
		if (requerie.length() > configuration.maxStrRequestLength)
			requerie.erase(requerie.length() - configuration.maxStrRequestLength);
		requestNumber = getRequests.size() + 1;
		vectorRequest.push_back(requerie);
		requerie = "";
		getRequests.emplace(requestNumber, vectorRequest);
		requestNumber = 1;
		nlohmann::json reqconfig, requerConfig;
		vector< nlohmann::json>strConfig;
		for (auto it = getRequests.begin(); it != getRequests.end(); it++)
		{
			for (const auto& iter : it->second)
			{
				requerie = iter;
				if (requerie.size() > 3)
				{
					strConfig.push_back({ requestNumber, requerie });
					requerie = "";
				}
				else
					requerie = "";
				reqconfig.clear();
			}
			requestNumber++;
		}
		requerConfig = { "Req", strConfig };
		strConfig.clear();
		ofstream reqfile("requests.json", ios::out | ios::trunc);
		if (!reqfile)
			cout << "\n " << "Requests file is not found.";
		reqfile << requerConfig;
		requerConfig.clear();
		reqfile.close();
		requerie = "";
		vectorRequest.clear();
	}
}

void ConverterJSON::wordSplitFunction(vector<string>& sentence, vector<string>& setWords)
{
	string highlightedWord = "";
	for (int ir = 0; ir < sentence.size(); ++ir)
	{
		for (int n = 0; n < sentence[ir].length(); ++n)
		{
			if (sentence[ir][n] != '-' && sentence[ir][n] != '\'' && sentence[ir][n] != '/' && sentence[ir][n] != ')' && sentence[ir][n] != '(' && sentence[ir][n] != ' ' && sentence[ir][n] != ';' && sentence[ir][n] != ':' && sentence[ir][n] != ',' && sentence[ir][n] != '.')
			{
				if (sentence[ir][n] >= 65 && sentence[ir][n] <= 90)
				{
					sentence[ir][n] += 32;
				}
				highlightedWord += sentence[ir][n];
			}
			else
			{
				if (highlightedWord.length() > 3)
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

void ConverterJSON::multiMapFillFunction(vector<Entry>& vectEntr, vector<Entry>& getWC, multimap<string, vector< Entry>>& countWordM)
{
	for (int ip = 0; ip < getWC.size(); ++ip)
	{
		int matchÑount = 0;
		for (int iq = 0; iq < getWC.size(); ++iq)
		{
			if ((getWC[ip].word == getWC[iq].word) && (matchÑount == 0))
			{
				vectEntr.push_back(getWC[iq]);
				matchÑount++;
			}
		}
		countWordM.insert({ getWC[ip].word, vectEntr });
		vectEntr.clear();
	}
	getWC.clear();
}

void ConverterJSON::vectorEntryFillFunction(vector<string> vectWord[DOCUMENT_NUMBER], vector<Entry> getWC[DOCUMENT_NUMBER], int filmId)//////////////////
{
	size_t wordRepetitionCount = 0;
	size_t entryCount = filmId;
	vector<string> checkRepetition;
	Entry* entry = new Entry[DOCUMENT_NUMBER];
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
		if ((pointer == end(checkRepetition)) && (vectWord[filmId][itr].size() > 3))
		{
			entry[entryCount].docId = filmId;
			entry[entryCount].freqWordsCount = wordRepetitionCount;
			entry[entryCount].word = vectWord[filmId][itr];
			getWC[filmId].push_back(entry[entryCount]);
		}
		checkRepetition.push_back(vectWord[filmId][itr]);
		wordRepetitionCount = 0;
	}
	delete[] entry;
	checkRepetition.clear();
}

void ConverterJSON::readingFromDatabase(queue<string>& q, mutex& mtx, condition_variable& cond, atomic<bool>& fileComplete)
{
	while (fileComplete) {
		unique_lock<mutex> lock(mtx);
		cond.wait(lock, [&] { return !q.empty() || !fileComplete; });
		if (q.empty()) {
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

void ConverterJSON::wordCountFunction(multimap<string, vector< Entry>>& countWordsMap)
{
	vector<string> wordsFromFilesVector[DOCUMENT_NUMBER], vectorWord[DOCUMENT_NUMBER];
	vector<Entry> getWordCount[DOCUMENT_NUMBER], vectorEntry[DOCUMENT_NUMBER];
	string strWord[DOCUMENT_NUMBER];
	string moviePath[DOCUMENT_NUMBER];
	ifstream file[DOCUMENT_NUMBER];
	thread film[DOCUMENT_NUMBER];
	mutex mtx;
	queue<string> lines;
	atomic<bool> fileComplete = true;
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
		vector<thread> threads;
		for (int i = 0; i < DOCUMENT_NUMBER; ++i)
		{
			threads.emplace_back(&ConverterJSON::readingFromDatabase, this, ref(lines), ref(mtx), ref(cond), ref(fileComplete));
		}
		wordSplitFunction(wordsFromFilesVector[i], vectorWord[i]);
		wordsFromFilesVector[i].clear();
		vectorEntryFillFunction(vectorWord, getWordCount, i);
		fileComplete = false;
		cond.notify_all();
		for (auto& thread : threads)
		{
			thread.join();
		}
		for (int ip = 0; ip < DOCUMENT_NUMBER; ++ip)
		{
			multiMapFillFunction(vectorEntry[i], getWordCount[i], countWordsMap);
			getWordCount[i].clear();
			vectorWord[i].clear();
		}
	}
}

void ConverterJSON::searchAnswerFunction(multimap<string, vector< Entry>>& countWordsMap, map <int, vector< string>> getRequest)
{
	vector<string> requestWord, reqtWord;
	int reqNumber = 1;
	nlohmann::json dataConfig[MAX_RESPONS], docConfig, requestNumberConfig, requestResultConfig, answersResultConfig;
	vector< nlohmann::json> resultVectorConfig, answersVectorConfig, numRequestConfig;
	for (auto iterator = getRequests.begin(); iterator != getRequests.end(); iterator++)
	{
		for (auto& wordsRequest : iterator->second)
		{
			reqtWord.push_back(wordsRequest);
		}
		reqtWord.push_back(";");
		wordSplitFunction(reqtWord, requestWord);
		reqtWord.clear();
		multimap<size_t, size_t> searchResult;
		size_t absoluteRelevance = 0,
			maxAbsoluteRelevanceDoc = 0,
			maxAbsoluteRelevance = 0,
			nlohmArrayCount = 0;
		for (int i = 0; i < requestWord.size(); ++i)
		{
			for (auto it = countWordsMap.begin(); it != countWordsMap.end(); ++it)
			{
				if (requestWord[i] == it->first)
				{
					for (auto m : it->second)
					{
						searchResult.insert({ m.freqWordsCount, m.docId });
						absoluteRelevance += m.freqWordsCount;
						if (absoluteRelevance > maxAbsoluteRelevance)
							maxAbsoluteRelevanceDoc = absoluteRelevance;
					}
				}
			}
			int countResponses = 0;
			for (auto iter = searchResult.crbegin(); iter != searchResult.crend(); ++iter)
			{
				if (countResponses < MAX_RESPONS)
				{
					if (iter->first > 0)
					{
						docConfig += { { "Document number", iter->second }, { "Number of matches",iter->first }, };
					}
				}
				else
				{
					searchResult.clear();
					break;
				}
				countResponses++;
			}
			if (absoluteRelevance > 0)
				dataConfig[i] = { { "word ", requestWord[i]},{"result", { absoluteRelevance > 0} }, {"Absolute relevance", absoluteRelevance},{"Relative relevance",0.}, { docConfig} };
			if (absoluteRelevance == 0)
			{
				dataConfig[i] = { { "word ", requestWord[i]}, {"result", { absoluteRelevance != 0}}, {"Absolute relevance", absoluteRelevance},{"Relative relevance",0.}, { docConfig == 0} };
			}
			docConfig.clear();
			absoluteRelevance = 0;
			searchResult.clear();
			if (i == requestWord.size() - 1)
				requestWord.clear();
			if (maxAbsoluteRelevanceDoc > maxAbsoluteRelevance)
				maxAbsoluteRelevance = maxAbsoluteRelevanceDoc;
		}
		double relativeRelevance = 0;
		for (int id = 0; id < MAX_RESPONS; ++id)
		{
			for (const auto& item : dataConfig[id].items())
			{
				relativeRelevance = (double)dataConfig[id][2].at(1) / (double)maxAbsoluteRelevance;
				relativeRelevance = round(relativeRelevance * 100) / 100;
				dataConfig[id][3].at(1) = relativeRelevance;
			}
			if (dataConfig[id] > 0)
			{
				requestResultConfig += { {dataConfig[id]}};
			}
			dataConfig[id].clear();
		}
		requestNumberConfig = { {reqNumber, {requestResultConfig} } };
		resultVectorConfig.push_back(requestNumberConfig);
		requestResultConfig.clear();
		requestNumberConfig.clear();
		searchResult.clear();
		++reqNumber;
	}
	answersVectorConfig.push_back({ "Answers", resultVectorConfig });
	ofstream ansfile("answers.json", ios::out | ios::trunc);
	if (!ansfile)
		cout << "\n " << "Requests file is not found.";
	ansfile << answersVectorConfig;
	ansfile.close();
}

void ConverterJSON::getAnswerFunction()
{
	ifstream ansfile("answers.json");
	if (!ansfile)
		cout << "\n " << "Requests file is not found.";
	nlohmann::json ansconfig;
	int nReq = 0;
	string val = "", key = "";
	vector<string> vecAnswer;
	if (ansfile.peek() != EOF)
	{
		ansfile.seekg(0);
		ansfile >> ansconfig;
		ansfile.close();
		for (const auto& item : ansconfig.items())
		{
			for (const auto& ir : item.value().items())
			{
				for (const auto& iq : ir.value().items())
				{
					for (const auto& it : iq.value().items())
					{
						for (const auto& im : it.value().items())
						{
							if (nReq % 2 > 0)
							{
								key = to_string(im.value());
								vecAnswer.push_back(key);
								key = "";
							}
							if (nReq % 2 == 0)
							{
								val = to_string(im.value());
								vecAnswer.push_back(val);
								val = "";
							}
							nReq++;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < vecAnswer.size(); ++i)
	{
		cout << "\n " << vecAnswer[i];
	}
}
