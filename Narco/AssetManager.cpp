#include"inc/AssetManager.h"
using namespace std::filesystem;
namespace NARCO
{
	AssetManager::AssetManager(const char* path)
		: mCurrentPath(path)
	{
		
		reloadPage();
		//mPageAssets.insert_or_assign
	}
	void AssetManager::AddAsset(const char* path)
	{
		long long hash = MakeHash(path);

		auto result = mAssets.find(hash);

		if (result != mAssets.end())
		{
			ExceptionWarning(E_INVALIDARG, "This asset has already been loaded.");
			return;
		}

		mAssets.insert_or_assign(hash, new Asset(path));		
	}
	void AssetManager::AddSystemAsset(const char* path)
	{
		long long hash = MakeHash(path);

		auto result = mSystemAssets.find(hash);

		if (result != mSystemAssets.end())
		{
			ExceptionWarning(E_INVALIDARG, "This asset has already been loaded.");
			return;
		}

		mSystemAssets.insert_or_assign(hash, new Asset(path));

	}
	void AssetManager::reloadPage()
	{
		for (auto& i : mPageAssets)
		{
			delete i.second;
		}
		mPageAssets.clear();
		
		std::stringstream stream;

		for (directory_iterator itr(mCurrentPath); itr != directory_iterator(); ++itr)
		{
			bool bDirectory = itr->is_directory();
			std::string filePath;

			stream << itr->path();

			filePath = stream.str();
			
			int slashIndex = filePath.find_last_of("\\") - 1;
			filePath = filePath.replace(filePath.begin() + slashIndex, filePath.begin() + slashIndex + 2, "/");

			
			long long hash = MakeHash(filePath);

#ifdef _DEBUG
			std::cout << filePath << std::endl;
#endif

			mPageAssets.insert_or_assign(hash, new Asset(filePath.c_str()));

			stream.str("");
			filePath = "";
			filePath.clear();
		}

	}
}