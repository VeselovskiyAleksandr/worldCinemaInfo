#include "gtest/gtest.h"
#include "ConverterJSON.h"
#include "Entry.h"
#include "Configuration.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

TEST(sampleCTestCase, sampleTest)
{
	EXPECT_EQ(1, 1);
}

	TEST(ConverterJSONTest, wordCountFunctionTest)
	{
		ConverterJSON converterJSON;
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


		vector<string> wordsFromFilesVectorTest[DOCUMENT_NUMBER], vectorWordTest[DOCUMENT_NUMBER];
		 wordsFromFilesVectorTest[0] =
{"\ndoc_id: ",
"0 ",
"NameMovie: Ivan Vasilyevich Changes Profession,",
"Country : USSR,", 
"DateCreation : 1973,",
"FilmDirector : Leonid Gaidai,",
"Scriptwriter : Leonid Gaidai, Vladlen Bakhnov,",
"FilmStudio : Mosfilm,",
"annotation : The film tells the story of an inventor(Aleksandr Demyanenko) who creates a time machine that opens a portal to the 16th century.He then accidentally swaps his apartment building manger, Ivan Vasilyevich Bunsha(Yuriy Yakovlev), with his namesakeand doppelganger, Tsar Ivan(Yuriy Yakovlev) the Terrible.Laugh follows laugh, as a bumbling Soviet building managerand the angry Tsar swap roles,",
"Aleksandr Demyanenko : Aleksandr 'Shurik' Timofeyev, inventor,",
"Yuriy Yakovlev : Ivan the Terrible / Ivan Vasilyevich Bunsha,",
"Leonid Kuravlyov : George Miloslavsky, burglar,",
"Natalia Selezneva : Zinaida, Shurik's wife,", 
"Natalia Krachkovskaya : Uliana Andreevna Bunsha,",
"Saveliy Kramarov : Feofan the clerk,",
"Natalia Kustinskaya : Yakin's mistress,", 
"Mikhail Pugovkin : film director Yakin,",
"Sergey Filippov : Swedish ambassador,",
"Vladimir Etush : Anton Semyonovich Shpak, dentist,"};
		 converterJSON.wordSplitFunction(wordsFromFilesVectorTest[0], vectorWordTest[0]);
			 				EXPECT_EQ(vectorWordTest[0][17], "gaidai");

		  vector<Entry> getWordCountTest[DOCUMENT_NUMBER];
		  converterJSON.vectorEntryFillFunction(vectorWordTest, getWordCountTest, 0);
				for (int i = 0; i < getWordCountTest[0].size(); ++i)
					{
						for (auto j : getWordCountTest[0])
							{
		                       if(j.word == "yakovlev")
							   EXPECT_EQ( j.freqWordsCount, 3);
							}
					}

		multimap<string, vector< Entry>> countWordsMapTest;
		vector<Entry> vectorEntryTest[DOCUMENT_NUMBER];
		converterJSON.multiMapFillFunction(vectorEntryTest[0], getWordCountTest[0], countWordsMapTest);

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
				converterJSON.searchAnswerFunction(countWordsMapTest, getRequestsTest);
				converterJSON.getAnswerFunction();
		}
int main()
{
	testing::InitGoogleTest();
		return RUN_ALL_TESTS();
}