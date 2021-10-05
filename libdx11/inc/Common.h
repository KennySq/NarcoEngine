#pragma once

#ifndef NARCO_API
	#define NARCO_API __declspec(dllexport)
#endif

#define PUBLIC_API public:
#define PRIVATE_API private:
#define PROTECTED_API protected:
#define PUBLIC_PROPERTY public:
#define PRIVATE_PROPERTY private:
#define PROTECTED_PROPERTY protected:

#pragma comment(lib, "DirectXTK.lib")


#include<Windows.h>
#include<iostream>
#include<string>
#include<sstream>
#include<exception>
#include<vector>
#include<map>
#include<unordered_map>
#include<typeinfo>
#include<fstream>
#include<filesystem>

#include<d3d11.h>
#include<dxgi.h>
#include<DirectXMath.h>
#include<DirectXColors.h>
#include<d3d11shader.h>
#include<wrl.h>

#include<Effects.h>

#include"CallStack.h"

using namespace Microsoft::WRL;
using namespace DirectX;

namespace NARCO
{
	typedef unsigned int uint;
	typedef unsigned int uint32;

	namespace Debug
	{
		NARCO_API void Throw(std::string description);
		NARCO_API void Log(std::string description);
	}

#ifdef _DEBUG

	template<typename _Ty>
	NARCO_API void NullCheck(_Ty* ptr)
	{
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

		if (ptr == nullptr)
		{
			SetConsoleTextAttribute(console, 12);

			std::cout << ":: WARNING :: " << ptr << " is nullptr.";

			SetConsoleTextAttribute(console, 15);
		}
	}
#endif

	NARCO_API long long MakeHash(std::string s);
	NARCO_API long long MakeHash(unsigned int i);
}