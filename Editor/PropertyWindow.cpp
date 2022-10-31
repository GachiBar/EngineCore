#include "PropertyWindow.h"
#include "SimpleMath.h"
#include "imgui/imgui.h"
#include "../GameplaySystem/Component.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include "../Editor/InputSystem/InputManager.h"

void PropertyWindow::draw_imgui(std::shared_ptr<engine::GameObject> go)
{
	ImGui::Begin("Property Window");
	if (!go.get())
	{
		ImGui::End();
		return;
	}

	for (size_t i = 0; i < go->Count(); ++i) 
	{
		auto component = (*go)[i];
		auto editablePropertiesNames = component->GetEditablePropertiesNames();

		for (auto propertyName : editablePropertiesNames) 
		{
			if (propertyName == "EditorEuler")
			{
				mono::mono_property_invoker t(component->GetInternal().get_type().get_property(propertyName));
				mono::mono_object t1(t.get_value(component->GetInternal()));
				auto t2 = t1.unbox<DirectX::SimpleMath::Vector3>();
				float t3[3] = { t2.x, t2.y,t2.z };
				if (ImGui::SliderFloat3("Euler Rotation", t3, -360.f, 360.f))
				{
					t.set_value(component->GetInternal(), t3);
				}
			}
			if (propertyName == "LocalScale")
			{
				mono::mono_property_invoker t(component->GetInternal().get_type().get_property(propertyName));
				mono::mono_object t1(t.get_value(component->GetInternal()));
				auto t2 = t1.unbox<DirectX::SimpleMath::Vector3>();
				float t3[3] = { t2.x, t2.y,t2.z };
				if (ImGui::InputFloat3("Scale", t3))
				{
					t.set_value(component->GetInternal(), t3);
				}
			}
			if (propertyName == "LocalPosition")
			{
				mono::mono_property_invoker t(component->GetInternal().get_type().get_property(propertyName));
				mono::mono_object t1(t.get_value(component->GetInternal()));
				auto t2 = t1.unbox<DirectX::SimpleMath::Vector3>();
				float t3[3] = { t2.x, t2.y,t2.z };
				if (ImGui::InputFloat3("Position", t3))
				{
					t.set_value(component->GetInternal(), t3);
				}
			}
		}
	}

	ImGui::End();
}
