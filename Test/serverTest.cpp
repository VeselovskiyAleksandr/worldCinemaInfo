#include "gtest/gtest.h"
#include "ConverterJSON.h"

using namespace std;

TEST(sampleCTestCase, sampleTest)
{
	EXPECT_EQ(1, 1);
}


	//TEST(ConverterJSON, sampleMultimap)
	//{
	//	ConverterJSON converterJSON;
	//	vector< string > requestsTest = { "Etush", "Vysotskiy", "Kuravlyov", "my" };
	//	pair<int, vector< string>> oPair(0, requestsTest);
	//	map <int, vector< string>>getRequests;
		//	getRequests.insert(oPair);
			//multimap<string, vector< Entry>> countWordsMap;

//		TEST(wordCountFunction, sampleMultimap)
	//	{
			//		EXPECT_EQ(converterJSON.countWordsMap.size(), 3);
	//	}
		//	for (const auto& pair : converterJSON.countWordsMap)
		//	{
			//	if (pair.first == "Etush")
		//		{
		//			for (const auto& p : pair.first)
		//			{
		//				if (p == entry.docId)
		//				EXPECT_EQ(entry.docId, 0);
		//				else if(p== entry.freqWordsCount)
		//					EXPECT_EQ(entry.freqWordsCount, 1);
		//			}
		//		}
		//		else if (pair.first == "Vysotskiy")
		//		{
		//			for (const auto& p : pair.first)
		//			{
		//				if (p == entry.docId)
		//					EXPECT_EQ(entry.docId, 2);
		//				else if (p == entry.freqWordsCount)
		//					EXPECT_EQ(entry.freqWordsCount, 2);
		//			}
		//		}
		//		else if (pair.first == "Kuravlyov")
		//		{?????????????????????????????????
		//			{
		//				if (p == entry.docId)
		//					EXPECT_TRUE(entry.docId, 0|| entry.docId, 1);
		//				else if (p == entry.freqWordsCount)
		//					EXPECT_EQ(entry.freqWordsCount, 2);
		//			}
		//		}
		//	}
		//	delete[] entry;
		//}
int main()
{
	testing::InitGoogleTest();
		return RUN_ALL_TESTS();
}