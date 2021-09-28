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

	class Asset
	{
	public:
		Asset(const char* path);
		Asset(const Asset& o) = delete;
		Asset(const Asset&& o) = delete;
		virtual ~Asset();

		eAssetType GetAssetType() const { return mAssetType; }
		const std::string& GetPath() const { return mPath; }
	private:

		static std::map<std::string, eAssetType> EXTENSION_MAP;

		eAssetType mAssetType;
		std::string mPath;
	};
}