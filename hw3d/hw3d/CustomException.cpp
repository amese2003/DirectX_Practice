#include "CustomException.h"
#include <sstream>
#include <string>

using namespace std;

CustomException::CustomException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* CustomException::what() const noexcept {
	ostringstream oss;
	oss << GetType() << endl << GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* CustomException::GetType() const noexcept {
	return "Custom Exception";
}

int CustomException::GetLine() const noexcept {
	return line;
}

const string& CustomException::GetFile() const noexcept {
	return file;
}

string CustomException::GetOriginString() const noexcept {
	ostringstream oss;
	oss << "[File] " << file << endl << "[Line] " << line;
	return oss.str();
}