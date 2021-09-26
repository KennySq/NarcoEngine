#pragma once

namespace NARCO
{
	class IFileSystem
	{
	public:

		virtual ~IFileSystem() {}

		virtual void SetPath(const char* path) = 0;
		virtual void Load() = 0;
		virtual void Reload() = 0;
		virtual void Release() = 0;
	};
}