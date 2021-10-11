#include"inc/Asset.h"
using namespace std;

namespace NARCO
{

	/*const char* Asset::EXTENSIONS_IMAGE[] = { ".png", ".jpg", ".jpeg", ".tga", ".gif" };
	const char* Asset::EXTENSIONS_MESH[] = { ".fbx", ".obj" };
	const char* Asset::EXTENSIONS_SOUND[] = { ".wav", ".mp3" };
	const char* Asset::EXTENSIONS_TEXT[] = { ".txt", ".xml", ".html" };*/



	Asset::Asset(const char* path)
		: mPath(path), mAssetType(ASSET_UNKNOWN)
	{
		std::string extension;

		int index = mPath.find_last_of('.');

		if(index < 0)
		{
			extension = "DIRECTORY";
		}
		else
		{
			extension = mPath.substr(index);

		}

		auto lamda_determineExt = [extension]() -> eAssetType
		{
			auto result = EXTENSION_MAP.find(extension);

			if (result != EXTENSION_MAP.end())
			{
				return result->second;
			}
		};

		mAssetType = lamda_determineExt();

	}
	Asset::~Asset()
	{
	}
}