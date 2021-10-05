#include"inc/Common.h"
#include"inc/CallStack.h"

namespace NARCO
{
	void Debug::Throw(std::string description)
	{
#ifdef _DEBUG
		
		std::cout << description << '\n';

		CallStack cs(8);
		cs.Print();
#endif
		
		throw description.c_str();

	}

	void Debug::Log(std::string description)
	{
#ifdef _DEBUG
		std::cout << "LOG :: " << description << '\n';

		CallStack cs(3);

		cs.Print();

#endif
	}

	NARCO_API long long MakeHash(std::string s)
	{
		const int p = 53;
		const int m = 1e9 + 9;

		int val = 0;
		int pow = 1;

		for (unsigned int i = 0; i < s.length(); i++)
		{
			val = (val + (s[i] - 'a' + 1) * pow) % m;
			pow = (p * pow) % m;
		}

		return val;
	}

	NARCO_API long long MakeHash(unsigned int i)
	{
		i = ((i >> 16) ^ i) * 0x45d9f3b;
		i = ((i >> 16) ^ i) * 0x45d9f3b;
		i = (i >> 16) ^ i;
		return i;
	}

}