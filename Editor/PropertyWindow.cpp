#include "PropertyWindow.h"
#include "../monowrapper/monopp/mono_method.h"
#include "../monowrapper/monopp/mono_method_invoker.h"
#include "../monowrapper/monopp/mono_property_invoker.h"
#include "../Editor/InputSystem/InputManager.h"
#include "SimpleMath.h"
#include "imgui/imgui_sugar.hpp"

void PropertyWindow::draw_imgui(std::shared_ptr<mono::mono_object> go)
{
	with_Window("Property window")
	{
		if (!go)
			return;
		mono::mono_property components_count_property = go->get_type().get_property("Count");
		mono::mono_property_invoker components_count_property_invoker(components_count_property);
		mono::mono_object components_count_object(components_count_property_invoker.get_value(*go));
		int components_count = components_count_object.unbox<int>();

		// Iterate over all game object's components.
		for (int j = 0; j < components_count; ++j)
		{
			mono::mono_method get_component_method = go->get_type().get_method("get_Item");
			mono::mono_method_invoker get_component_method_invoker(get_component_method);

			void* args[1];
			args[0] = &j;

			mono::mono_object component(get_component_method_invoker.invoke(*go, args));
			auto properties = component.get_type().get_properties();

			for (auto& property : properties)
			{
				if (property.get_name() == "Euler")
				{
					mono::mono_property_invoker t(property);
					mono::mono_object t1(t.get_value(component));
					auto t2 = t1.unbox<DirectX::SimpleMath::Vector3>();
					float t3[3] = {t2.x, t2.y, t2.z};
					if (ImGui::SliderFloat3("Euler Rotation", t3, -360.f, 360.f))
					{
						t.set_value(component, t3);
					}
				}
				if (property.get_name() == "Scale")
				{
					mono::mono_property_invoker t(property);
					mono::mono_object t1(t.get_value(component));
					auto t2 = t1.unbox<DirectX::SimpleMath::Vector3>();
					float t3[3] = {t2.x, t2.y, t2.z};
					if (ImGui::InputFloat3("Scale", t3))
					{
						t.set_value(component, t3);
					}
				}
			}
		}
	}
}
