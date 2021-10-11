#pragma once
#include"Common.h"

namespace NARCO
{
	enum eAssetType
	{
		ASSET_IMAGE,
		ASSET_TEXT,
		ASSET_MESH,
		ASSET_SOUND,
		ASSET_DIRECTORY,
		ASSET_UNKNOWN,
	};

	static std::map<std::string, eAssetType> EXTENSION_MAP =
	{
		std::pair<std::string, eAssetType>(".png\"", ASSET_IMAGE),
		std::pair<std::string, eAssetType>(".jpg\"", ASSET_IMAGE),
		std::pair<std::string, eAssetType>(".jpeg\"", ASSET_IMAGE),
		std::pair<std::string, eAssetType>(".tga\"", ASSET_IMAGE),
		std::pair<std::string, eAssetType>(".gif\"", ASSET_IMAGE),

		std::pair<std::string, eAssetType>(".fbx\"", ASSET_MESH),
		std::pair<std::string, eAssetType>(".obj\"", ASSET_MESH),

		std::pair<std::string, eAssetType>(".wav\"", ASSET_SOUND),
		std::pair<std::string, eAssetType>(".mp3\"", ASSET_SOUND),

		std::pair<std::string, eAssetType>(".txt\"", ASSET_TEXT),
		std::pair<std::string, eAssetType>(".xml\"", ASSET_TEXT),
		std::pair<std::string, eAssetType>(".html\"", ASSET_TEXT),

		std::pair<std::string, eAssetType>("DIRECTORY", ASSET_DIRECTORY)

	};

	class Asset
	{
	public:
		Asset(const char* path);
		Asset(const Asset& o) = delete;
		Asset(const Asset&& o) = delete;
		virtual ~Asset();

		eAssetType GetAssetType() const { return mAssetType; }
		const std::string& GetPath() const { return mPath; }

		static eAssetType DetermineType(std::string extension)
		{
			auto result = EXTENSION_MAP.find(extension);

			if (result == EXTENSION_MAP.end())
			{
				Debug::Log("Not supported.");
				return ASSET_UNKNOWN;
			}

			return result->second;
		}
	private:

		eAssetType mAssetType;
		std::string mPath;
	};
}