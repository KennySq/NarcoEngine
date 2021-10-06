#include"../inc/GUI_Material.h"

namespace NARCO
{
	GUI_Material::GUI_Material(Material* material)
		: mMaterial(material)
	{
		
		
		
	}
	GUI_Material::~GUI_Material()
	{
	}
	NARCO_API void GUI_Material::Start()
	{
		static auto& constRegisters = mMaterial->GetBuffers();
		static auto& textureRegisters = mMaterial->GetTextures();
		static auto& unorders = mMaterial->GetUnorders();

		static char buf[64] = "";
		
		D3D11_BUFFER_DESC bufDesc{};

		for (uint i = 0; i < constRegisters.size(); i++)
		{

			//eMaterialInputType type = eMaterialInputType::INPUT_MATRIX4X4;
			auto buffer = constRegisters[i].Get();
			buffer->GetDesc(&bufDesc);
			uint size = bufDesc.ByteWidth;

			// some data (various size)
			float* bytes = new float[size / sizeof(float)];
			
			MaterialInput input(bytes, size);

			mConstantBytes.insert_or_assign(buffer, input);

		}
	}
	NARCO_API void GUI_Material::Update()
	{
		static auto& constRegisters = mMaterial->GetBuffers();
		static auto& textureRegisters = mMaterial->GetTextures();
		static auto& unorders = mMaterial->GetUnorders();
		
		static char buf[64] = "";
		static D3D11_BUFFER_DESC bufferDesc{};
		

		for (uint i = 0; i < constRegisters.size(); i++)
		{
			auto buffer = constRegisters[i].Get();
			uint size;
			const MaterialInput& input = mConstantBytes[buffer];

			buffer->GetDesc(&bufferDesc);
			
			size = bufferDesc.ByteWidth;
			float* data = reinterpret_cast<float*>(input.Data);

			static float mat[4][4];

			for (uint j = 0; j < size; j += 64)
			{
				const char* label1 = (std::string("float_comp_0") + std::to_string(j+1)).c_str();
				const char* label2 = (std::string("float_comp_1") + std::to_string(j+2)).c_str();
				const char* label3 = (std::string("float_comp_2") + std::to_string(j+3)).c_str();
				const char* label4 = (std::string("float_comp_3") + std::to_string(j+4)).c_str();

				ImGui::InputFloat4(label1, mat[0]);
				ImGui::InputFloat4(label2, mat[1]);
				ImGui::InputFloat4(label3, mat[2]);
				ImGui::InputFloat4(label4, mat[3]);

				for (uint t = 0; t < 4; t++)
				{
					data[0  + t] = mat[0][t]; // 0 1 2 3
					data[4  + t] = mat[1][t]; // 4 5 6 7
					data[8  + t] = mat[2][t]; // 8 9 10 11
					data[12 + t] = mat[3][t]; // 12 13 14 15
				}
			}
				
			mMaterial->UpdateConstant(data, i);
		}
		return;
	}
	NARCO_API void GUI_Material::Draw()
	{
		return;
	}
	NARCO_API void GUI_Material::End()
	{
		return;
	}
}