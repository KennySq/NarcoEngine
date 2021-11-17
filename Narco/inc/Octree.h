#pragma once
#include"Common.h"

namespace NARCO
{
	template<typename _Ty>
	struct Octree
	{
		template<typename _Ty>
		struct Node
		{
			_Ty Value;

			Node<_Ty>* Lower[8];
			Node<_Ty>* Higher;
		};

	public:
		Octree();
	private:
		Node* mRoot;
		unsigned int mCount;
	
		std::unordered_map<size_t, Node<_Ty>*> mMap;
	};
}