// worldCinemaInfo.cpp: определяет точку входа для приложения.
//
#include "ConverterJSON.h"
#include "EngineStart.h"
#include "Configuration.h"
#include "RequestsJSON.h"
#include "AnswersJSON.h"

int main()
{
	ConverterJSON converterJSON;
	EngineStart engineStart;
	Configuration configuration;
	RequestsJSON requestsJSON;
	AnswersJSON answersJSON;
	try
	{
		engineStart.start(configuration);
	}
	catch (const exception& x)
	{
		cerr << "\nConfiguration file is not found. Fix the problem.";
		return 1;
	}
//	if (engineStart.isJsonValid("config.json"))
	if (engineStart.isJsonValid("requests.json"))
	{
		cout << "\nrequests.json is correct!\n";
	}
	else
	{
		cout << "\nrequests.json contains errors!\n";
	}
	if (engineStart.isJsonValid("answers.json"))
	{
		cout << "\nanswers.json is correct!";
	}
	else
	{
		cout << "\nanswers.json contains errors!\n";
	}
	engineStart.openFile("requests.json");
	engineStart.openFile("answers.json");
	requestsJSON.getRequestsFunction(requestsJSON.getRequests);
	//requestsJSON.requerInputFunction(requestsJSON.getRequests);
	requestsJSON.writeMapToFileFunction(requestsJSON.getRequests);
	auto startTime = chrono::high_resolution_clock::now();
	converterJSON.readFromFilesFunction(converterJSON.wordsFromFilesVector, configuration);
	converterJSON.getWordDataFunction(converterJSON.countWordsMap, configuration, converterJSON.wordsFromFilesVector);
	answersJSON.searchAnswerFunction(converterJSON.countWordsMap, requestsJSON.getRequests);
	answersJSON.getAnswerFunction();
	auto endTime = chrono::high_resolution_clock::now();
	chrono::duration<double> serial_duration = endTime - startTime;
	cout << "\nQuery execution time: " << serial_duration.count() << " seconds";
	return 0;
}
