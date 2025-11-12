#include "RequestsJSON.h"

template<typename Iterator, typename vectorJSON>
void RequestsJSON::selectingWordsMinimumLength(const Iterator &iter, Configuration& configuration, vectorJSON& strConfig, size_t& requestNumber)
{
	string requerie = iter;
	if (requerie.size() > configuration.minWordLength)
	{
		strConfig.push_back({ requestNumber, requerie });
		requerie = "";
	}
	else
		requerie = "";
}

template<typename vectorJSON>
void RequestsJSON::mapTraversalFunction(map<size_t, vector<string>>& getRequests, vectorJSON& strConfig)
{
	size_t nReq=1;
	for (auto& it = getRequests.begin(); it != getRequests.end(); it ++ )
	{
		for (const auto& iter : it->second)
		{
			selectingWordsMinimumLength(iter, configuration, strConfig, nReq);
		}
		nReq++;
	}
}

void RequestsJSON::requerInputFunction(map<size_t, vector<string>>& getRequests)
{
	if (getRequests.size() < configuration.maxRequest)
	{
		cout << "\n" << "                                      Information about domestic and foreign films" << "\n" << "\n";
		cout << "               Search query field" << "\n" << "\n";
		vector<string> vectorRequest;
		string requerie = "";
		size_t requestNumber = 1;
		getline(cin, requerie);
		if (requerie.length() > configuration.maxStrRequestLength)
			requerie.erase(requerie.length() - configuration.maxStrRequestLength);
		requestNumber = getRequests.size() + 1;
		vectorRequest.push_back(requerie);
		requerie = "";
		getRequests.emplace(requestNumber, vectorRequest);
		vectorRequest.clear();
	}
}
void RequestsJSON::writeMapToFileFunction(map<size_t, vector<string>>& getRequests)
{
	nlohmann::json requerConfig;
	vector< nlohmann::json>strConfig;
	mapTraversalFunction(getRequests, strConfig);
	requerConfig = { "Req", strConfig };
	strConfig.clear();
	ofstream reqfile("requests.json", ios::out | ios::trunc);
	if (!reqfile)
		cout << "\n " << "Requests file is not found.";
	reqfile << requerConfig;
	requerConfig.clear();
	reqfile.close();
}

