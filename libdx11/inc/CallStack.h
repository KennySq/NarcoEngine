#pragma once
#include<Windows.h>
#include<iostream>
#include<string>
#include<DbgHelp.h>
#include<sstream>

#pragma comment(lib, "dbghelp.lib")

struct Symbol : public SYMBOL_INFO
{
	Symbol()
	{
		MaxNameLen = 256U;
		SizeOfStruct = sizeof(SYMBOL_INFO);
	}

	char mBuffer[256U];
};

class CallStack
{
public:
	static const size_t mMaxStackDepth = 16U;

	CallStack();
	
	__declspec(dllexport) void Print();

	unsigned long GetHash() const { return mHash; }
	size_t GetCount() const { return mCount; }


	void* operator[] (const size_t index) const
	{
		if (index >= mMaxStackDepth || index < 0)
		{
			return nullptr;
		}

		return mAddresses[index];
	}


private:

	std::string GetSymbolName(void* address) const;



	void* mAddresses[mMaxStackDepth];
	

	unsigned long mHash;
	size_t mCount;

	HANDLE mSymbolHandle;


};


