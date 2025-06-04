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

TEST(ConverterJSONTest, getRequestsFunctionTest)
{
	ConverterJSON converterJSON;
	map <int, vector< string>>getRequestsTest;
	converterJSON.getRequestsFunction(getRequestsTest);
	for (auto i : getRequestsTest)
	{
		if (i.first == 2)
		for(auto j: i.second)
		{
			EXPECT_TRUE(j== "tank this other");
		}
	}
}

	TEST(ConverterJSONTest, wordCountFunctionTest)
	{
		ConverterJSON converterJSON;
		Entry entry;
		vector< Entry> vectorEntry;
	//	vector< string > requestsTest = { "Etush", "Vysotskiy", "Kuravlyov", "my"};
//		pair<int, vector< string>> oPair(0, requestsTest);
//		map <int, vector< string>>getRequestsTest;
	//	EXPECT_EQ(getRequestsTest.size(), 0);
	//		getRequests.insert(oPair);


		multimap<string, vector< Entry>> countWordsMap;


//		converterJSON.getRequestsFunction(getRequestsTest);
//		ASSERT_EQ(getRequestsTest.size(), 0);
//		EXPECT_FALSE(getRequestsTest.size()== 5);
//		EXPECT_EQ(getRequestsTest.size(), 0);


			converterJSON.wordCountFunction(countWordsMap);


//		TEST(wordCountFunction, sampleMultimap)
//		{

	//				EXPECT_EQ(countWordsMapTest.size(), 940);
	//	}


//			for (const auto& pair : countWordsMap)
//			{
//				if (pair.first == "Etush")
//				{
//					for (const auto& p : pair.first)
//					{
	//					if (p == entry.docId)
	//					EXPECT_TRUE(entry.docId== 0);
	//					else if(p== entry.freqWordsCount)
	//						EXPECT_TRUE(entry.freqWordsCount== 1);
//					}
//				}
//				else if (pair.first == "Vysotskiy")
//				{
//					for (const auto& p : pair.first)
//					{
//						if (p == entry.docId)
//							EXPECT_TRUE(entry.docId== 2);
//						else if (p == entry.freqWordsCount)
//							EXPECT_TRUE(entry.freqWordsCount== 2);
//					}
//				}
//				else if (pair.first == "Kuravlyov")
//					for (const auto& p : pair.first)
//					{
//						if (p == entry.docId)
//							EXPECT_TRUE(entry.docId== 0|| entry.docId== 1);
//						else if (p == entry.freqWordsCount)
//							EXPECT_TRUE(entry.freqWordsCount== 2);
//					}
	//			}
	//				delete entry;
		}
int main()
{
	testing::InitGoogleTest();
		return RUN_ALL_TESTS();
}