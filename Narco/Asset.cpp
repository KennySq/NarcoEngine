#include"inc/Asset.h"
using namespace std;

namespace NARCO
{

	/*const char* Asset::EXTENSIONS_IMAGE[] = { ".png", ".jpg", ".jpeg", ".tga", ".gif" };
	const char* Asset::EXTENSIONS_MESH[] = { ".fbx", ".obj" };
	const char* Asset::EXTENSIONS_SOUND[] = { ".wav", ".mp3" };
	const char* Asset::EXTENSIONS_TEXT[] = { ".txt", ".xml", ".html" };*/

	map<std::string, eAssetType> Asset::EXTENSION_MAP =
	{
		pair<string, eAssetType>(".png\"", ASSET_IMAGE),
		pair<string, eAssetType>(".jpg\"", ASSET_IMAGE),
		pair<string, eAssetType>(".jpeg\"", ASSET_IMAGE),
		pair<string, eAssetType>(".tga\"", ASSET_IMAGE),
		pair<string, eAssetType>(".gif\"", ASSET_IMAGE),

		pair<string, eAssetType>(".fbx\"", ASSET_MESH),
		pair<string, eAssetType>(".obj\"", ASSET_MESH),

		pair<string, eAssetType>(".wav\"", ASSET_SOUND),
		pair<string, eAssetType>(".mp3\"", ASSET_SOUND),

		pair<string, eAssetType>(".txt\"", ASSET_TEXT),
		pair<string, eAssetType>(".xml\"", ASSET_TEXT),
		pair<string, eAssetType>(".html\"", ASSET_TEXT),

		pair<string, eAssetType>("DIRECTORY", ASSET_DIRECTORY)

	};

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