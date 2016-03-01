#pragma once

#include <string>

std::string WChar2Ansi(LPCWSTR pwszSrc);

std::wstring Ansi2WChar(LPCSTR pszSrc);