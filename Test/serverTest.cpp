#include "gtest/gtest.h"
#include "ConverterJSON.h"
#include "Entry.h"
#include "Configuration.h"
#include <map>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

TEST(sampleCTestCase, sampleTest)
{
	EXPECT_EQ(1, 1);
}

	TEST(ConverterJSONTest, wordCountFunctionTest)
	{
		ConverterJSON converterJSON;
		Configuration configuration;
		map <int, vector< string>>getRequestsTest;
		converterJSON.getRequestsFunction(getRequestsTest);
		for (auto i : getRequestsTest)
		{
			if (i.first == 2)
				for (auto j : i.second)
				{
					EXPECT_TRUE(j == "tank this other");
				}
		}
		converterJSON.openFile("config.json");
		EXPECT_TRUE("config.json");
		converterJSON.start();
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
		vector<string> wordsFromFilesVectorTest , vectorWordTest[DOCUMENT_NUMBER], wordsFromFilesTest[DOCUMENT_NUMBER];
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
			 else if(i==8)
				 EXPECT_EQ(wordsFromFilesVectorTest[i], "Yuriy Yakovlev: Ivan the Terrible/Ivan Vasilyevich Bunsha, ");
		 }
		 wordsFromFilesTest[0]=wordsFromFilesVectorTest;

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
		  converterJSON.vectorEntryFillFunction(vectorWordTest, getWordCountTest, 0);
				for (int i = 0; i < getWordCountTest[0].size(); ++i)
					{
						for (auto j : getWordCountTest[0])
							{
		                       if(j.word == "yakovlev")
							   EXPECT_EQ( j.freqWordsCount, 3);
							   else if(j.word == "yakin")
							   EXPECT_EQ(j.freqWordsCount, 2);
							   else if (j.word == "shurik")
							   EXPECT_EQ(j.freqWordsCount, 2);
							}
					}
		converterJSON.getRequestsFunction(getRequestsTest);
		multimap<string, vector< Entry>> countWordsMapTest;
		vector<Entry> vectorEntryTest[DOCUMENT_NUMBER];
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
				converterJSON.multiMapFillFunction(vectorEntryTest[0], getWordCountTest[0], countWordsMapTest);
					for (const auto q : countWordsMapTest)
					{
						if (q.first == "Ivan")
						{
							for (const auto s : q.second)
							{
								EXPECT_EQ(s.freqWordsCount, 5);
								cout << "\n9999999999    " << s.word << "     " << s.freqWordsCount;
							}
						}
						else if (q.first == "inventor")
						{
							for (const auto s : q.second)
							{
								EXPECT_EQ(s.freqWordsCount, 2);
							}
						}
						else if (q.first == "feofan")
						{
							for (const auto s : q.second)
							{
								EXPECT_EQ(s.freqWordsCount, 1);
							}
						}
						else if (q.first == "miloslavsky")
						{
							for (const auto s : q.second)
							{
								EXPECT_EQ(s.freqWordsCount, 1);
							}
						}
						else if (q.first == "kramarov")
						{
							for (const auto s : q.second)
							{
								EXPECT_EQ(s.freqWordsCount, 1);
							}
						}
					}
				converterJSON.searchAnswerFunction(countWordsMapTest, getRequestsTest);
				converterJSON.getAnswerFunction();
				getRequestsTest.clear();
				wordsFromFilesVectorTest.clear();
				vectorWordTest[DOCUMENT_NUMBER].clear(); 
				wordsFromFilesTest[DOCUMENT_NUMBER].clear();
				getWordCountTest[DOCUMENT_NUMBER].clear();
				countWordsMapTest.clear();
				vectorEntryTest[DOCUMENT_NUMBER].clear();
		}
int main()
{
	testing::InitGoogleTest();
		return RUN_ALL_TESTS();
}