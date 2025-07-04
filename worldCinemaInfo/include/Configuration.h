#pragma once

#include <iostream>
#include <string>
#include <map>

using namespace std;

#define MAX_SIZE 10000 //���������� ����� ���������� ���� � ����������
#define MAX_RESPONS 5 //���������� ����� �������
#define DOCUMENT_NUMBER 10 //���������� ���������� � ����

struct Configuration
{
	string name; //c������� �������� ������
	string version; //������ ������
	map<int, string> movieTitles;
	static const int maxRequest = 1000; //������������ ���������� �������
	static const int maxStrRequestLength = 80; //���������� ���������� ������ � �������
	static const int numberProcessorCores = 4; //���������� ���� ����������
	static const int minWordLength = 3; //���������� ����� �� ��������� ����� ������������
};