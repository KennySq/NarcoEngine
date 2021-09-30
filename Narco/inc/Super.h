#pragma once

#include<Common.h>
#include<D3DHardware.h>

namespace NARCO
{
	class Super
	{
	
	protected:
	
		Super() : mDevice(D3DHW::GetDevice()), mContext(D3DHW::GetImmediateContext()) {}
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mContext;
	};
}