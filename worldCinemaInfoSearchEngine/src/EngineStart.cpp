#include "EngineStart.h"

bool EngineStart::isJSONValid(const string& filepath) {
	try {
		ifstream file(filepath);
		nlohmann::json data = nlohmann::json::parse(file);
		return true;
	}
	catch (const nlohmann::json::parse_error& err) {
		cerr << "JSON Error: " << err.what();
		return false;
	}
}

void EngineStart::start(Configuration& configuration)
{
	cout << "                              Launching a search engine... " << "\n";
	nlohmann::json config;
	ifstream cnfile("config.json");
	if (!cnfile)
	{
		throw exception();
	}
	if (cnfile.peek() != EOF)
	{
		cnfile >> config;
		cnfile.close();
		config["config"]["maxResponses"] = { MAX_RESPONS };
		cout << config["config"] << "\n";
		for (const auto& item : config["config"].items())
		{
			if (item.key() == "name")
			{
				configuration.name = item.value();
			}
			else if (item.key() == "version")
			{
				configuration.version = item.value();
			}
		}
		for (const auto& item : config["files"].items())
		{
			int id = stoi(item.key());
			pair<int, string> oPair(id, item.value());
			configuration.movieTitles.insert(oPair);
		}
	}
};

void EngineStart::openFile(string path)
{
	if (filesystem::exists(path))
	{
		if (path == "answers.json")
		{
			ofstream file("answers.json", ios::out | ios::trunc);
			file.close();
		}
	}
	else
	{
		ofstream file;
		file.close();
	}
};