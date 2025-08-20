#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include "Configuration.h"
#include <filesystem>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;


class EngineStart
{
public: EngineStart() = default;
	  Configuration configuration;

	  //функция проверки на корректность структуры JSON-файла
	  bool isJsonValid(const std::string& filepath);

	  //функция начала работы движка
	  void start(Configuration& configuration);

	  //функция проверки открытия файла
	  void openFile(string path);
};