#include "stdafx.h"
#include "str_tools.h"

using namespace std;

std::string WChar2Ansi( LPCWSTR pwszSrc )
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen<= 0) return std::string("");
	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen -1] = 0;
	std::string strTemp(pszDst);
	delete [] pszDst;
	return strTemp;
}

std::wstring Ansi2WChar(LPCSTR pszSrc)
{
	int nLen = strlen(pszSrc);
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if(nSize <= 0) return NULL;
	WCHAR *pwszDst = new WCHAR[nSize+1];
	if( NULL == pwszDst) return NULL;
	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;
	if( pwszDst[0] == 0xFEFF) // skip Oxfeff
		for(int i = 0; i < nSize; i ++) 
			pwszDst[i] = pwszDst[i+1]; 
	wstring wcharString(pwszDst);
	delete pwszDst;
	return wcharString;
}