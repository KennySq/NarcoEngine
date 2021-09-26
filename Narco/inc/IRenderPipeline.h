#pragma once
#include"Common.h"
#include"D3DUtil.h"

namespace NARCO
{
	typedef class IRenderPipeline
	{
	public:
		virtual void Stage_InputAssembler() = 0;
		
		virtual void Stage_VertexShader() = 0;
		virtual void Stage_GeometryShader() = 0;
		virtual void Stage_DomainShader() = 0;
		virtual void Stage_HullShader() = 0;
		virtual void Stage_PixelShader() = 0;
		
		virtual void Stage_Draw() = 0;

		virtual void Stage_RasterizerState() = 0;
		virtual void Stage_OutputMerger() = 0;



		virtual void Start() = 0;
		virtual void Update(float delta) = 0;
		virtual void Render(float delta) = 0;
		virtual void Release() = 0;

	private:

	} IRP;
}