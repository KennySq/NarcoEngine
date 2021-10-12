#include"../inc/GUI_GameObject.h"

namespace NARCO
{
	NARCO_API GUI_GameObject::GUI_GameObject(GameObject* gameObject)
		: mGameObject(gameObject)
	{
	}
	NARCO_API void GUI_GameObject::Start()
	{

		return;
	}
	NARCO_API void GUI_GameObject::Update()
	{
		const auto& components = mGameObject->GetComponents();

		for (auto c : components)
		{
			Component* comp = c.second;
			std::string name = comp->GetName();
			name = name.substr(name.find_last_of(':') + 1);

			static bool bHeader = false;

			if (ImGui::CollapsingHeader(name.c_str(), bHeader))
			{
				ComponentID id = c.second->GetID();
				Component* comp = c.second;

				if (id == typeid(Transform).hash_code())
				{
					guiTransform(static_cast<Transform*>(comp));
				}
			}



		}

		return;
	}
	NARCO_API void GUI_GameObject::Draw()
	{
		return;
	}
	NARCO_API void GUI_GameObject::End()
	{
		return;
	}
	NARCO_API void GUI_GameObject::guiTransform(Transform* transform)
	{
		static float guiMat[3][3] = { {transform->GetPosition().m128_f32[0],transform->GetPosition().m128_f32[1], transform->GetPosition().m128_f32[2]},
										{transform->GetRotation().m128_f32[0], transform->GetRotation().m128_f32[1], transform->GetRotation().m128_f32[2]}
			,{1,1,1} };

		const GameObject* root = transform->GetRoot();

		std::string name = root->GetName();

		std::string floatList0 = (name + "_0").c_str();
		std::string floatList1 = (name + "_1").c_str();
		std::string floatList2 = (name + "_2").c_str();

		bool bFloat0 = ImGui::InputFloat3(floatList0.c_str(), guiMat[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
		bool bFloat1 = ImGui::InputFloat3(floatList1.c_str(), guiMat[1], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
		bool bFloat2 = ImGui::InputFloat3(floatList2.c_str(), guiMat[2], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

		if (bFloat0 || bFloat1 || bFloat2)
		{
			XMVECTOR originPosition = transform->GetPosition();
			XMVECTOR originRotation = transform->GetRotation();
			XMVECTOR originScale = transform->GetScale();

			XMVECTOR position = { guiMat[0][0], guiMat[0][1], guiMat[0][2], 1.0f };
			XMVECTOR rotation = { XMConvertToRadians(guiMat[1][0]), XMConvertToRadians(guiMat[1][1]), XMConvertToRadians(guiMat[1][2]), 1.0f };
			XMVECTOR scale	  = { guiMat[2][0], guiMat[2][1], guiMat[2][2], 1.0f };
			
			rotation = XMQuaternionRotationRollPitchYaw(rotation.m128_f32[0], rotation.m128_f32[1], rotation.m128_f32[2]);
			
			XMMATRIX mat = XMMatrixAffineTransformation(scale, originRotation, rotation, originPosition - position);
			transform->SetMatrix(mat);
		}
		

	}
}