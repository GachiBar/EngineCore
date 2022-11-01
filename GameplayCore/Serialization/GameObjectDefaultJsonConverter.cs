using System;
using System.Collections.Generic;
using System.Reflection;
using GameplayCore.Components;
using Newtonsoft.Json;

namespace GameplayCore.Serialization
{
    public class GameObjectDefaultJsonConverter : JsonConverter<GameObject>
    {
        private Scene _scene;

        public GameObjectDefaultJsonConverter(Scene scene = null)
        {
            _scene = scene;
        }
        
        private IEnumerable<FieldInfo> FilterFields(FieldInfo[] fields, Func<FieldInfo, bool> filter)
        {
            foreach (var field in fields)
            {
                if (filter(field))
                    yield return field;
            }
        }

        private bool HasAttribute(FieldInfo fieldInfo, Type type)
        {
            foreach (var attribute in fieldInfo.CustomAttributes)
            {
                if (attribute.AttributeType == type)
                    return true;
            }

            return false;
        }

        public override void WriteJson(JsonWriter writer, GameObject gameObject, JsonSerializer serializer)
        {
            writer.WriteStartObject();

            writer.WritePropertyName("Guid");
            writer.WriteValue(gameObject.Guid.ToString());

            writer.WritePropertyName("Components");
            writer.WriteStartArray();

            foreach (var component in gameObject.GetComponentsEnumerable())
            {
                writer.WriteStartObject();
                writer.WritePropertyName("Type");

                writer.WriteValue(component.GetType().ToString());

                foreach (var field in FieldsIterator(component))
                {
                    // Passing reference guid instead of serializing it 
                    if (typeof(GameObject).IsAssignableFrom(field.FieldType))
                    {
                        writer.WritePropertyName(field.Name);
                        serializer.Serialize(writer, (field.GetValue(component) as GameObject)?.Guid);
                        continue;
                    }
                    
                    // Passing reference guid instead of serializing it 
                    if (typeof(Component).IsAssignableFrom(field.FieldType))
                    {
                        writer.WritePropertyName(field.Name);
                        serializer.Serialize(writer, (field.GetValue(component) as Component)?.GameObject.Guid);
                        continue;
                    }
                    
                    // Actual serialization
                    writer.WritePropertyName(field.Name);
                    serializer.Serialize(writer, field.GetValue(component));
                }

                writer.WriteEndObject();
            }

            writer.WriteEndArray();
            writer.WriteEndObject();
        }

        private IEnumerable<FieldInfo> FieldsIterator(Component component)
        {
            FieldInfo[] fields = component
                .GetType()
                .GetFields(BindingFlags.Public | BindingFlags.Instance);

            foreach (var field in FilterFields(fields, info
                         => !HasAttribute(info, typeof(HideInInspectorAttribute))))
            {
                yield return field;
            }

            fields = component
                .GetType()
                .GetFields(BindingFlags.NonPublic | BindingFlags.Instance);

            foreach (var field in FilterFields(fields, info
                         => HasAttribute(info, typeof(SerializeFieldAttribute))))
            {
                yield return field;
            }
        }

        private string GetComponentGuid(Component component)
        {
            FieldInfo field = component.GetType().GetField("_gameObject");
            return field.GetValue(component).ToString();
        }

        public override GameObject ReadJson(JsonReader reader, Type objectType, GameObject existingValue,
            bool hasExistingValue,
            JsonSerializer serializer)
        {
            if (reader.TokenType != JsonToken.StartObject)
                throw new JsonException("Can't get start of object!");
            
            // First stage without reference set
            ReadCheckPropertyName("Guid");
            string guid = ReadString();

            ReadCheckPropertyName("Components");

            reader.Read();
            if (reader.TokenType != JsonToken.StartArray)
                throw new JsonException("Can't read start of components array");

            List<Component> components = new List<Component>();

            while (reader.Read())
            {
                if (reader.TokenType == JsonToken.EndArray)
                {
                    break;
                }
                
                if (reader.TokenType != JsonToken.StartObject)
                    throw new JsonException("Can't read start of component data");
                
                ReadCheckPropertyName("Type");
                string typeName = ReadString();
                Type componentType = Type.GetType(typeName);
                Component current = Activator.CreateInstance(componentType) as Component;
                
                if(current == null)
                    throw new JsonException("Can't create component from read type");

                while (reader.Read())
                {
                    if (reader.TokenType != JsonToken.PropertyName)
                        break;

                    string fieldName = reader.Value as string;

                    FieldInfo field = componentType.GetField(fieldName);
                    
                    if(field == null)
                    {
                        // Just skip it
                        // or throw new JsonException();
                        reader.Read();
                        continue;
                    }

                    if (typeof(Component).IsAssignableFrom(field.FieldType)
                        || typeof(GameObject).IsAssignableFrom(field.FieldType))
                    {
                        // Just skip it as it has guid reference
                        reader.Read();
                        continue;
                    }

                    if (field.FieldType == typeof(UInt16) || field.FieldType == typeof(UInt32) || field.FieldType == typeof(UInt64))
                    {
                        reader.Read();
                        UInt64.TryParse(reader.Value.ToString(), out var value);
                        field.SetValue(current, value);
                    }
                    else
                    {
                        reader.Read();
                        object fieldValue = serializer.Deserialize(reader);
                        field.SetValue(current, fieldValue);
                    }
                }
                
                components.Add(current);
            }
            
            reader.Read();
            if (reader.TokenType != JsonToken.EndObject)
                throw new JsonException($"Can't read end of object. Got type \"{reader.TokenType}\" and value \"{reader.Value}\" ");
            
            // Create gameObject and return it
            GameObject gameObject = new GameObject(_scene, System.Guid.Parse(guid));

            MethodInfo addComponent = gameObject.GetType()
                .GetMethod("AddComponentSilent", BindingFlags.Instance | BindingFlags.NonPublic);
            
            foreach (var component in components)
            {
                object[] parameters = {component};
                addComponent.Invoke(gameObject, parameters);
            }
            
            return gameObject;

            void ReadCheckPropertyName(string propertyName)
            {
                reader.Read();
                if (reader.TokenType != JsonToken.PropertyName)
                    throw new JsonException($"Can't read property name: {propertyName}");
            
                string name = reader.Value as string;
                if(propertyName != name)
                    throw new JsonException($"Unexpected property name has been read: {name}");
            }

            string ReadString()
            {
                reader.Read();
                return reader.Value as string;
            }
        }
    }
}