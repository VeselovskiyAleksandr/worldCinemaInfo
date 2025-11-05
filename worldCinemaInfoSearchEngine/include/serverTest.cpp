#include "gtest/gtest.h"
#include "ConverterJSON.h"
#include "Entry.h"
#include "Configuration.h"
#include "EngineStart.h"
#include "RequestsJSON.h"
#include <map>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

TEST(sampleTestCase, sampleTest)
{
	EXPECT_EQ(1, 1);
}

TEST(RequestsJSONTest, requestsFunctionsTest)
{
	RequestsJSON requestsJSON;
	map <size_t, vector< string>>getRequestsTest;
	requestsJSON.getRequestsFunction(getRequestsTest);
	for (auto i : getRequestsTest)
	{
		if (i.first == 2)
			for (auto j : i.second)
			{
				EXPECT_TRUE(j == "tank this other");
			}
	}
	getRequestsTest.clear();
}

TEST(ConverterJSONTest, someFunctionsTest)
{
	ConverterJSON converterJSON;
	Configuration configuration;
	nlohmann::json config;
	ifstream cnfile("config.json");
	if (!cnfile)
	{
		cerr << "\n" << "The cnfile is not found." << "\n";
	}
	if (cnfile.peek() != EOF)
	{
		cnfile >> config;
		cnfile.close();
		for (const auto& item : config["files"].items())
		{
			int id = stoi(item.key());
			pair<int, string> oPair(id, item.value());
			configuration.movieTitles.insert(oPair);
		}
	}
	string strWordTest;
	string moviePath;
	ifstream resfile;
	for (const auto& it : configuration.movieTitles)
	{
		if (it.first == 1)
			moviePath = it.second;
		resfile.open(moviePath);
	}
	if (!resfile.is_open())
	{
		cerr << "\n" << "The file " << configuration.movieTitles[0] << " is not found." << "\n";
	}
	vector<string> wordsFromFilesVectorTest, vectorWordTest[DOCUMENT_NUMBER], wordsFromFilesTest[DOCUMENT_NUMBER];
	while (getline(resfile, strWordTest))
	{
		wordsFromFilesVectorTest.push_back(strWordTest);
		strWordTest = "";
	}
	moviePath = "";
	for (int i = 0; i < wordsFromFilesVectorTest.size(); ++i)
	{
		if (i == 3)
			EXPECT_EQ(wordsFromFilesVectorTest[i], "FilmDirector: Leonid Gaidai,");
		else if (i == 8)
			EXPECT_EQ(wordsFromFilesVectorTest[i], "Yuriy Yakovlev: Ivan the Terrible/Ivan Vasilyevich Bunsha, ");
	}
	wordsFromFilesTest[0] = wordsFromFilesVectorTest;

	converterJSON.wordSplitFunction(wordsFromFilesTest[0], vectorWordTest[0]);
	for (int j = 0; j < vectorWordTest[0].size(); ++j)
	{
		if (j == 25)
			EXPECT_EQ(vectorWordTest[0][25], "demyanenko");
		else if (j == 113)
			EXPECT_EQ(vectorWordTest[0][113], "shpak");
		else if (j == 78)
			EXPECT_EQ(vectorWordTest[0][78], "kuravlyov");
	}
	vector<Entry> getWordCountTest[DOCUMENT_NUMBER];
	multimap<string, vector< Entry>> countWordsMapTest;
	vector<Entry> getCountWordsTest[DOCUMENT_NUMBER];
	converterJSON.vectorEntryFillFunction(vectorWordTest, 0, getCountWordsTest);
	for (int i = 0; i < getWordCountTest[0].size(); ++i)
	{
		for (auto j : getWordCountTest[0])
		{
			if (j.word == "yakovlev")
				EXPECT_EQ(j.freqWordsCount, 3);
			else if (j.word == "yakin")
				EXPECT_EQ(j.freqWordsCount, 2);
			else if (j.word == "shurik")
				EXPECT_EQ(j.freqWordsCount, 2);
		}
	}
	for (multimap<string, vector< Entry>>::const_iterator pair = countWordsMapTest.begin(); pair != countWordsMapTest.end(); ++pair)
	{
		if (pair->first == "zinaida")
		{

			for (auto& p : pair->second)
			{
				EXPECT_EQ(p.freqWordsCount, 1);
			}
		}
		else if (pair->first == "Demyanenko")
		{
			for (const auto& p : pair->second)
			{
				EXPECT_EQ(p.freqWordsCount, 2);
			}
		}
		else if (pair->first == "Gaidai")
		{
			for (const auto& p : pair->second)
			{
				EXPECT_EQ(p.freqWordsCount, 4);
			}
		}
	}
	wordsFromFilesVectorTest.clear();
	countWordsMapTest.clear();
	for (int i = 0; i < DOCUMENT_NUMBER; ++i)
	{
		vectorWordTest[i].clear();
		wordsFromFilesTest[i].clear();
		getWordCountTest[i].clear();
	}
	config.clear();
}


int main()
{
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}