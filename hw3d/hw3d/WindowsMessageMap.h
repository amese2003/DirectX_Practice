#pragma once
#include <unordered_map>
#include <Windows.h>
#include <string>

using namespace std;

class WindowsMssageMap {
private:
	unordered_map<DWORD, string> map;
public:
	WindowsMssageMap() {}
	string operator() (DWORD msg, LPARAM lp, WPARAM wp) const;
};