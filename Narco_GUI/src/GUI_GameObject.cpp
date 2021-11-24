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
	void GUI_GameObject::Refresh()
	{



	}
	NARCO_API void GUI_GameObject::guiTransform(Transform* transform)
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			mGuiMatrix[0][i] = transform->GetPosition().m128_f32[i];
		}

		for (unsigned int i = 0; i < 3; i++)
		{
			mGuiMatrix[1][i] = XMConvertToDegrees(transform->GetRotation().m128_f32[i]);
		}

		for (unsigned int i = 0; i < 3; i++)
		{
			mGuiMatrix[2][i] = transform->GetScale().m128_f32[i];
		}

		const GameObject* root = transform->GetRoot();

		std::string name = root->GetName();

		std::string floatList0 = (name + "_0").c_str();
		std::string floatList1 = (name + "_1").c_str();
		std::string floatList2 = (name + "_2").c_str();


		bool bFloat0 = ImGui::InputFloat3(floatList0.c_str(), mGuiMatrix[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
		bool bFloat1 = ImGui::InputFloat3(floatList1.c_str(), mGuiMatrix[1], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
		bool bFloat2 = ImGui::InputFloat3(floatList2.c_str(), mGuiMatrix[2], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

		if (bFloat0 || bFloat1 || bFloat2)
		{
			XMVECTOR originPosition = transform->GetPosition();
			XMVECTOR originRotation = transform->GetRotation();
			XMVECTOR originScale = transform->GetScale();

			XMVECTOR position = { mGuiMatrix[0][0], mGuiMatrix[0][1], mGuiMatrix[0][2], 1.0f };
			XMVECTOR rotation = { XMConvertToRadians(mGuiMatrix[1][0]), XMConvertToRadians(mGuiMatrix[1][1]), XMConvertToRadians(mGuiMatrix[1][2]), XMConvertToRadians(mGuiMatrix[1][3]) };
			XMVECTOR scale	  = { mGuiMatrix[2][0], mGuiMatrix[2][1], mGuiMatrix[2][2], 1.0f };
			
			//rotation = XMQuaternionRotationRollPitchYaw(rotation.m128_f32[0], rotation.m128_f32[1], rotation.m128_f32[2]);
			//XMQuaternionRotationRollPitchYaw(mGuiMatrix[1][0], mGuiMatrix[1][1], mGuiMatrix[1][2]);
			XMMATRIX mat = XMMatrixAffineTransformation(scale, XMQuaternionIdentity(), XMQuaternionIdentity(), position);
			//mat *= XMMatrixRotationQuaternion(rotation);
			
			//auto rot = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(mGuiMatrix[1][0]),
			//	XMConvertToRadians(mGuiMatrix[1][1]), XMConvertToRadians(mGuiMatrix[1][2]));

			//for (uint i = 0; i < 3; i++)
			//{
			//	mGuiMatrix[0][i] = mat.r[3].m128_f32[i];
			//}

			//for (uint i = 0; i < 3; i++)
			//{
			//	mGuiMatrix[1][i] =
			//}

			transform->SetMatrix(mat);
		}
		

	}
}