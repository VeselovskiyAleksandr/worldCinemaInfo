#include "RequestsJSON.h"

template<typename JsonIterator>
void RequestsJSON::conditionCheckingFunction(
	map<int, vector<string>>& getRequests,
	vector<string>& vecRequest,
	int& nReq,
	const typename JsonIterator& element)
{	
	try {
		string strValue = element.value().is_string()
			? element.value().get<string>()
			: to_string(element.value());
		if (strValue.size() >= 2 && strValue.front() == '"' && strValue.back() == '"') {
			strValue = strValue.substr(1, strValue.size() - 2);
		}
		if (!strValue.empty() && strValue != "Req")
		{
			vecRequest.push_back(strValue);
			getRequests.emplace(nReq, vecRequest);
			vecRequest.clear();
			nReq++;
		}
	}
	catch (const exception& e) {
		cerr << "\nError processing element: " << e.what();
	}
}

template<typename JsonType>
void RequestsJSON::vectorTraversalFunction(
	map<int, vector<string>>& getRequests,
	vector<string>& vecRequest,
	int& nReq, const JsonType& data)
{
	for (const auto& item : data.items()) {
		for (const auto& itr : item.value().items()) {
			conditionCheckingFunction(getRequests, vecRequest, nReq, itr);
		}
	}
}

template<typename Iterator>
void RequestsJSON::selectingWordsMinimumLength(string requerie, const typename Iterator&iter, Configuration& configuration)
{
	requerie = iter;
	if (requerie.size() > configuration.minWordLength)
	{
		strConfig.push_back({ requestNumber, requerie });
		requerie = "";
	}
	else
		requerie = "";
	reqconfig.clear();
}

template<typename JsonIterator, typename VectorJSON>
void RequestsJSON::vectorTraversalFunction2(
	map<int, vector<string>>& getRequests,
	vector<string>& vecRequest, string requerie,
	size_t& nReq, const typename JsonIterator& element,
	 VectorJSON& strConfig)
{
	for (auto it = getRequests.begin(); it != getRequests.end(); it ++ )
	{
		for (const auto& iter : it->second)
		{
			requerie = iter;
			if (requerie.size() > configuration.minWordLength)
			{
				if constexpr (std::is_same_v<typename VectorJSON::value_type, nlohmann::json>)
				{
					// Для вектора JSON-объектов
					strConfig.push_back({ {"id", nReq}, {"query", requerie} });
				}
				else
				{
					// Для других типов векторов
					strConfig.push_back(requerie);
				}
				strConfig.push_back({ nReq, requerie });
				requerie = "";
			}
			else
				requerie = "";
		}
	}
}


void RequestsJSON::getRequestsFunction(map<int, vector <string>>& getRequests)
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
		vectorTraversalFunction(getRequests, vecRequest, nReq, reqconfig);
	}
}

void RequestsJSON::requerInputFunction(map<int, vector<string>>& getRequests)
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
		requestNumber = 1;
		nlohmann::json reqconfig, requerConfig;
		vector< nlohmann::json>strConfig;
	//    vectorTraversalFunction2(getRequests, vectorRequest,requerie, requestNumber,  reqconfig, strConfig);
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
