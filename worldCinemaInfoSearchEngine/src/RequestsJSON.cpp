#include "RequestsJSON.h"

template <typename JsonIterator>
void RequestsJSON::conditionCheckingFunction(map <size_t, vector<string>> &getRequests, vector<string>& vecRequest, size_t& nReq, const JsonIterator& element)
{	
		string strValue = element.value().is_string() ? element.value().get<string>() : to_string(element.value());
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
void RequestsJSON::JsonTraversalFunction(map<size_t, vector<string>>& getRequests, vector<string>& vecRequest, size_t& nReq, const JsonType& data)
{
	for (const auto& item : data.items())
	{
		for (const auto& itr : item.value().items())
		{
					conditionCheckingFunction(getRequests, vecRequest, nReq, itr);
		}
	}
}

template<typename Iterator, typename vectorJSON>
void RequestsJSON::selectingWordsMinimumLength(const Iterator &iter, Configuration& configuration, vectorJSON& strConfig, size_t requestNumber)
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

void RequestsJSON::getRequestsFunction(map<size_t, vector <string>>& getRequests)
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
		JsonTraversalFunction(getRequests, vecRequest, nReq, reqconfig);
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

