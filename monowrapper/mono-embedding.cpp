#include <iostream>

#include "monopp/mono_domain.h"
#include "monopp/mono_field.h"
#include "monopp/mono_field_invoker.h"
#include "monopp/mono_jit.h"
#include "monopp/mono_method.h"
#include "monopp/mono_method_invoker.h"
#include "monopp/mono_object.h"
#include "monopp/mono_property.h"
#include "monopp/mono_property_invoker.h"
#include "monopp/mono_type.h"

void PrintAssemblyTypes(const mono::mono_assembly& assembly)
{
    auto names = assembly.dump_type_names();

    for (auto& name : names)
    {
        printf("%s\n", name.c_str());
    }
}

void debug_class_info(const mono::mono_assembly& assembly, const std::string& class_namespace, const std::string& class_name)
{
    /// Get access to a c# type
    auto type = assembly.get_type(class_namespace, class_name);

    /// Prints out Tests
    std::cout << type.get_namespace() << std::endl;

    /// Prints out MonoppTest
    std::cout << type.get_name() << std::endl;

    /// Prints out Tests.MonoppTest
    std::cout << type.get_fullname() << std::endl;

    /// Checks and gets the base type of Tests.MonoppTest
    if(type.has_base_type())
    {
        auto base_type = type.get_base_type();
        std::cout << base_type.get_fullname() << std::endl;
    }
}

void debug_method_info(const mono::mono_method& method)
{
    std::cout << "Name: " << method.get_name() << "/n"
    << "Full name: " << method.get_fullname()<< "/n"
    << "Full declarative name: " << method.get_full_declname() << "/n"
    << "Visibility: " << mono::to_string(method.get_visibility()) << "/n"
    << "Is static: " << method.is_static() << "/n"
    << "Parameters: ";
    for(auto& type: method.get_param_types())
    {
        std::cout << type.get_name() << " ";
    }
    std::cout << "/n"
    << "Return type: " << method.get_return_type().get_name() << "/n";
}

void TryCallMethods(mono::mono_type& type)
{
    /// Create an instance of it. Default constructed.
    auto instance = type.new_instance();

    /// There are several ways of getting access to methods
    /// Way 1, name + arg count
    auto void_method = type.get_method("PrintFloatVar", 0);

    // Two other ways of getting access to methods
    // auto method2 = type.get_method("Method2(string)");
    // auto method3 = mono::make_method_invoker<std::string(std::string, int)>(type, "Method5");

    /// You can invoke it by creating a thunk and calling it passing
    /// the object it belongs to as the first parameter. Not passing
    /// an object as the first parameter will treat it as a static method
    auto thunk1 = mono::make_method_invoker<void()>(void_method);
    thunk1(instance);

    auto float_method = type.get_method("IncrementFloatVar", 1);
    auto thunk2 = mono::make_method_invoker<void(float)>(float_method);
    thunk2(instance, 5.f);

    // Static methods
    auto static_int_method = type.get_method("GetIncremented", 1);
    auto thunk3 = mono::make_method_invoker<int(int)>(static_int_method);
    auto result = thunk3(55); // Call without passing instance
    std::cout << result << std::endl;

    /// You can catch exceptions like so
    try
    {
        mono::make_method_invoker<int(int, float)>(type, "NonExistingFunction");
    }
    catch(const mono::mono_exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void TryFieldsAndProperties(mono::mono_type& type)
{
    /// Create an instance of it. Default constructed.
    auto instance = type.new_instance();

    auto field = type.get_field("MyPublicFloatVar");
    /// In order to get or set values to a field you need
    /// to create an invoker.
    auto mutable_field = mono::make_field_invoker<int>(field);
    auto field_value = mutable_field.get_value(instance); // If it's static then you don't need to pass object

    if(field.get_visibility() == mono::visibility::vis_public)
    {
        std::cout << "Field is public, so editor can set value\n";
        mutable_field.set_value(instance, 55);
    }
    
    std::cout << "Field value: " << field_value << std::endl;

    // Property
    auto property = type.get_property("someProperty");

    // Handle values through property invoker
    auto property_invoker = mono::make_property_invoker<int>(property);
    property_invoker.set_value(instance, 57);
    int value = property_invoker.get_value(instance);
    std::cout << "Property value: " << field_value << std::endl;

    // Handles values through getter and setter as methods
    auto setter = property.get_set_method();
    auto getter = property.get_get_method();

    auto setter_invoker = mono::make_method_invoker<int(int)>(setter);
    setter_invoker(instance, value - 3);
    
    auto getter_invoker = mono::make_method_invoker<int()>(getter);
    value = getter_invoker(instance);
    std::cout << "Property value: " << field_value << std::endl;
}

int main()
{
    if(!mono::init_with_mono_assembly_path("mono/lib", "RootDomain"))
        return 1;

    std::cout << "Mono initialised\n";

    
    /// Create an app domain. When destructed it will
    /// unload all loaded assemblies
    const mono::mono_domain my_domain("AppDomain");

    std::cout << "AppDomain created\n";
    
    /// Sets the current domain
    mono::mono_domain::set_current_domain(my_domain);
    std::cout << "AppDomain was set as current\n";
    
    const auto assembly = my_domain.get_assembly("MonoPlayground.dll");

    std::cout << "Got dll assembly\n";
    
    PrintAssemblyTypes(assembly);
    debug_class_info(assembly, "MonoPlayground", "CSharpTesting");

    auto testing_type = assembly.get_type("MonoPlayground", "CSharpTesting");
    TryCallMethods(testing_type);

    const auto method = testing_type.get_method("PrintFloatVar", 0);
    debug_method_info(method);
    
    TryFieldsAndProperties(testing_type);

    mono::shutdown();
}