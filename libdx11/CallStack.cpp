#include "inc/CallStack.h"

CallStack::CallStack(unsigned int count)
	: mSymbolHandle(GetCurrentProcess())

{
	SymInitialize(mSymbolHandle, nullptr, true);
	SymSetOptions(SYMOPT_LOAD_LINES);

	mCount = CaptureStackBackTrace(0, count, mAddresses, &mHash);
}

extern "C" __declspec(dllexport) void CallStack::Print()
{
	for (unsigned int i = 0; i < mCount; i++)
	{
		std::string str = GetSymbolName(mAddresses[i]);

		std::cout << str << "\n";
	}
}

std::string CallStack::GetSymbolName(void* address) const
{
	if (address == nullptr)
	{
		return "Failed.";
	}

	DWORD displacement = 0;
	DWORD64 address64 = reinterpret_cast<DWORD64>(address);

	Symbol symbol;

	IMAGEHLP_LINE64 line;

	line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	bool result = SymFromAddr(mSymbolHandle, address64, 0, &symbol);

	std::stringstream stream;
	
	result = SymGetLineFromAddr64(mSymbolHandle, address64, &displacement, &line);
	
	if (result == true)
	{
		stream << "\t" << line.FileName << "(" << line.LineNumber << ")" << symbol.Name;
	}
	else
	{
		stream << "\tEmpty info : " << symbol.Name;
	}

	return stream.str();
}
