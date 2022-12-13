using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
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

        public override void WriteJson(JsonWriter writer, GameObject gameObject, JsonSerializer serializer)
        {
            writer.WriteStartObject();
            
            JsonExtensions.WriteStringProperty(writer, "Guid", gameObject.Guid.ToString());

            writer.WritePropertyName("Components");
            writer.WriteStartArray();

            foreach (var component in gameObject)
            {
                writer.WriteStartObject();
                JsonExtensions.WriteStringProperty(writer, "Type", component.GetType().ToString());

                foreach (var field in ReflectionExtensions.SerializableFieldsIterator(component))
                {
                    // Passing reference guid instead of serializing it 
                    if (typeof(GameObject).IsAssignableFrom(field.FieldType))
                    {
                        System.Guid? guid = (field.GetValue(component) as GameObject)?.Guid;
                        JsonExtensions.WriteSerializedProperty(writer, serializer, field.Name, guid);
                        continue;
                    }

                    // Passing reference guid instead of serializing it 
                    if (typeof(Component).IsAssignableFrom(field.FieldType))
                    {
                        System.Guid? guid = (field.GetValue(component) as Component)?.GameObject.Guid;
                        JsonExtensions.WriteSerializedProperty(writer, serializer, field.Name, guid);
                        continue;
                    }

                    // Actual serialization
                    JsonExtensions.WriteSerializedProperty(writer, serializer, field.Name, field.GetValue(component));
                }

                writer.WriteEndObject();
            }

            writer.WriteEndArray();
            writer.WriteEndObject();
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
                return null;

            // First stage without reference set
            string guid = JsonExtensions.ReadString(reader, "Guid");
            
            JsonExtensions.ReadCheckPropertyName(reader, "Components");
            if (!JsonExtensions.CheckToken(reader, JsonToken.StartArray))
                return null;

            List<Component> components = new List<Component>();

            while (reader.Read())
            {
                if (reader.TokenType == JsonToken.EndArray)
                    break;

                if (reader.TokenType != JsonToken.StartObject)
                    return null;
                
                string typeName = JsonExtensions.ReadString(reader, "Type");
                Type componentType = Type.GetType(typeName);
                Component current = Activator.CreateInstance(componentType) as Component;

                if (current == null)
                {
                    Console.WriteLine("Can't create component from read type");
                    return null;
                }

                while (reader.Read())
                {
                    if (reader.TokenType != JsonToken.PropertyName)
                        break;

                    string fieldName = reader.Value as string;

                    FieldInfo field = componentType.GetField(fieldName, BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
                    Type temp = componentType.BaseType;
                    
                    while (field == null && temp != typeof(Component))
                    {                        
                        field = temp.GetField(fieldName, BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
                        temp = temp.BaseType;
                    }

                    if (field == null)
                    {
                        // Just skip it
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

                    if (field.FieldType == typeof(UInt16) || field.FieldType == typeof(UInt32) ||
                        field.FieldType == typeof(UInt64))
                    {
                        reader.Read();

                        if (field.FieldType == typeof(UInt16))
                        {
                            UInt16.TryParse(reader.Value.ToString(), out var value);
                            field.SetValue(current, value);
                        }
                        else if (field.FieldType == typeof(UInt32))
                        {
                            UInt32.TryParse(reader.Value.ToString(), out var value);
                            field.SetValue(current, value);
                        }
                        else
                        {
                            UInt64.TryParse(reader.Value.ToString(), out var value);
                            field.SetValue(current, value);
                        }
                    }
                    else
                    {
                        reader.Read();
                        object fieldValue = serializer.Deserialize(reader, field.FieldType);
                        field.SetValue(current, fieldValue);
                    }
                }
                if (reader.TokenType != JsonToken.EndObject)
                    return null;

                components.Add(current);
            }

            if (!JsonExtensions.CheckEndObject(reader))
                return null;

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
        }

        public void PassGuidReferences(string json)
        {
            JsonTextReader reader = new JsonTextReader(new StringReader(json));
            JsonSerializer serializer = JsonSerializer.CreateDefault();

            if (!JsonExtensions.CheckToken(reader, JsonToken.StartArray))
            {
                Console.WriteLine("Can't find start of GameObjects array");
                return;
            }

            while (reader.Read())
            {
                if (reader.TokenType == JsonToken.EndArray)
                    break;

                PassReference(reader, serializer);
            }
        }

        private void PassReference(JsonReader reader, JsonSerializer serializer)
        {
            if (reader.TokenType != JsonToken.StartObject)
                throw new JsonException("Can't get start of object!");

            // First stage without reference set
            JsonExtensions.ReadCheckPropertyName(reader, "Guid");
            string guid = JsonExtensions.ReadString(reader);
            GameObject actual = _scene.First(x => x.Guid.Equals(Guid.Parse(guid)));

            if (actual == null)
                throw new JsonException("Can't find serialized GameObject");

            JsonExtensions.ReadCheckPropertyName(reader, "Components");

            if (!JsonExtensions.CheckToken(reader, JsonToken.StartArray))
            {
                Console.WriteLine("Can't find start of GameObjects array");
                return;
            }

            while (reader.Read())
            {
                if (reader.TokenType == JsonToken.EndArray)
                    break;
                
                if (reader.TokenType != JsonToken.StartObject)
                    throw new JsonException("Can't read start of component data");

                JsonExtensions.ReadCheckPropertyName(reader, "Type");
                string typeName = JsonExtensions.ReadString(reader);
                Type componentType = Type.GetType(typeName);
                Component current = actual.GetComponent(componentType);
                
                if (current == null)
                    throw new JsonException("Can't find serialized component in GameObject");

                while (reader.Read())
                {
                    if (reader.TokenType != JsonToken.PropertyName)
                        break;

                    string fieldName = reader.Value as string;

                    FieldInfo field = componentType.GetField(fieldName,
                        BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);

                    if (field == null)
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
                        string guid_ref = reader.Value as string;
                        if(guid_ref == null)
                            continue;
                        
                        GameObject referencesGameObject = _scene.First(x => x.Guid.Equals(Guid.Parse(guid_ref)));

                        if (typeof(GameObject).IsAssignableFrom(field.FieldType))
                        {
                            field.SetValue(current, referencesGameObject);
                        }
                        else
                        {
                            Component referencedComponent = referencesGameObject.GetComponent(field.FieldType);
                            field.SetValue(current, referencedComponent);
                        }

                        continue;
                    }
                    
                    // Just skip it
                    reader.Read();
                    serializer.Deserialize(reader);
                }
            }

            if (!JsonExtensions.CheckToken(reader, JsonToken.EndObject))
            {
                throw new JsonException(
                    $"Can't read end of object. Got type \"{reader.TokenType}\" and value \"{reader.Value}\" ");
            }
        }
    }
}