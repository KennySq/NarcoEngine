#pragma once
#include<Common.h>

#include"Asset.h"

namespace NARCO
{
	class AssetManager
	{
	public:
		AssetManager(const char* path);


		void AddAsset(const char* path);
		void AddSystemAsset(const char* path);
		
		const std::map<long long, Asset*>& GetPageAssets() const { return mPageAssets; }
		const std::map<long long, Asset*>& GetSystemAssets() const { return mSystemAssets; }
		
		const std::string& GetCurrentPath() const { return mCurrentPath; }

	private:

		void reloadPage();

		std::map<long long, Asset*> mPageAssets;

		std::map<long long, Asset*> mAssets;
		std::map<long long, Asset*> mSystemAssets;
		std::string mCurrentPath;
	};
}