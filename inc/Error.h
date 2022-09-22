#pragma once 

#include <vector>
#include <string>
#include <functional>

void throwWarning(std::string from, std::string message, std::function<void()> callback = std::function<void()>());
void throwWarningWithChoice(std::string from, std::string message, std::string option0, std::string option1, std::function<void(int)> callback = std::function<void(int)>());