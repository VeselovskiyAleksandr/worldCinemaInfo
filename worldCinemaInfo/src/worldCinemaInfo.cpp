// worldCinemaInfo.cpp: определяет точку входа для приложения.
//

#include "ConverterJSON.h"

using namespace std;

int main()
{
	ConverterJSON converterJSON;
	try
	{
		converterJSON.start();
	}
	catch (const exception& x)
	{
		cerr << "The file is not found.";
		return 1;
	}
	converterJSON.openFile("requests.json");
	converterJSON.openFile("answers.json");
	converterJSON.getRequestsFunction(converterJSON.getRequests);
	//		converterJSON.requerInputFunction(converterJSON.getRequests);
	auto start_time = chrono::high_resolution_clock::now();
	converterJSON.wordCountFunction(converterJSON.countWordsMap);
	converterJSON.searchAnswerFunction(converterJSON.countWordsMap, converterJSON.getRequests);
	converterJSON.getAnswerFunction();
	auto end_time = chrono::high_resolution_clock::now();
	chrono::duration<double> serial_duration = end_time - start_time;
	cout << "\nQuery execution time: " << serial_duration.count() << " seconds";
	return 0;
}
