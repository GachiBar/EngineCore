using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using GameplayCore.Serialization;
using Newtonsoft.Json;

namespace GameplayCore.Resources
{
    public class MetadataReader
    {
        private static string BasePath = "Assets";
        private static ResourcesHolder _holder = new ResourcesHolder();
        
        public static Resource Create(string path)
        {
            string filepath = GetResourcePath(path);
            Debug($"Creating metadata of {filepath}!");

            FileType assetType = GetType(filepath);
            Type classType = GetClassType(assetType);
            if (classType == null)
            {
                Console.WriteLine("Can't get class type of resource!");
                return null;
            }
                

            System.Guid guid = Guid.NewGuid();
            object[] args = {filepath, guid};
            
            Resource resource = Activator.CreateInstance(classType, args) as Resource;
            Write(resource);
            _holder.Set(resource);

            Debug(resource.ToString());
            return resource;
        }

        public static Resource Read(string path)
        {
            Debug($"Reading metadata of {path}!");
            if (!File.Exists(GetMetaPath(path)))
                return Create(path);

            string metadataPath = GetMetaPath(path);

            if (_holder.TryGet(ReadOnlyGuid(metadataPath), out var value))
                return value;
            
            FileStream file = File.OpenRead(metadataPath);
            StreamReader reader = new StreamReader(file);
            Resource resource = Deserialize(reader.ReadToEnd(), path);
            file.Close();

            _holder.Set(resource);
            Debug($"Resource is {resource}");
            return resource;
        }

        private static System.Guid ReadOnlyGuid(string path)
        {
            if (!File.Exists(GetMetaPath(path)))
                return System.Guid.Empty;
            
            FileStream file = File.OpenRead(GetMetaPath(path));
            StreamReader stream = new StreamReader(file);
            JsonReader reader = new JsonTextReader(stream);

            while (reader.Read())
            {
                if (reader.TokenType == JsonToken.PropertyName &&
                    string.Equals((string) reader.Value, "_guid"))
                    break;
            }

            if (reader.TokenType != JsonToken.PropertyName)
            {
                file.Close();
                return System.Guid.Empty;
            }

            reader.Read();
            file.Close();
            
            return Guid.Parse(reader.Value as string ?? string.Empty);
        }

        // For tests only!
        [Obsolete]
        public static Resource ReadWithoutSave(string path)
        {
            Debug($"Reading metadata of {path}!");
            if (!File.Exists(GetMetaPath(path)))
                return Create(path);

            FileStream file = File.OpenRead(GetMetaPath(path));
            StreamReader reader = new StreamReader(file);
            Resource resource = Deserialize(reader.ReadToEnd(), path);
            file.Close();

            Debug($"Resource is {resource}");
            return resource;
        }

        public static void Write(Resource resource)
        {
            FileStream file = File.OpenWrite(GetMetaPath(resource.FilePath));
            StreamWriter writer = new StreamWriter(file);
            string data = Serialize(resource);
            writer.Write(data);
            writer.Dispose();
            file.Close();
        }

        public static Resource TryGetByGuid(System.Guid guid)
        {
            if (guid.Equals(System.Guid.Empty)) 
                return null;

            foreach (var pair in IterateAllResourcesFast())
            {
                if (pair.Item1.Equals(guid))
                    return Read(pair.Item2);
            }

            return null;
        }

        public static string CreateMaterial(string basepath)
        {
            // Create new Material()
            Material material = new Material();
            
            // Serialize -> string json
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = {new ResourceGuidJsonConverter()}
            };
            string json = JsonConvert.SerializeObject(material, options);
            
            // Generate material name
            int number = 0;
            Func<int, string> materialPath = (i) => Path.Combine(basepath, "material" + i.ToString() + ".material");
            while (File.Exists(materialPath(number)))
                number++;

            string path = materialPath(number);
            Console.WriteLine($"Material filename: {path}");

            // Write json in file
            FileStream file = File.OpenWrite(path);
            StreamWriter writer = new StreamWriter(file);
            writer.Write(json);
            writer.Dispose();
            file.Close();
            
            // Generate material metadata
            Create(path);

            return path;
        }

        private static IEnumerable<(System.Guid, string)> IterateAllResourcesFast()
        {
            string path = Path.Combine(Path.GetFullPath("."), BasePath);
            Debug($"Full assets path is {path}");
            if (Directory.Exists(path))
            {
                foreach (string file in Directory.EnumerateFiles(path , "*.*", SearchOption.AllDirectories))
                {
                    if(Path.GetExtension(file).Equals(".meta")) continue;

                    yield return (ReadOnlyGuid(file), file);
                }    
            }
        }
        
        private static IEnumerable<Resource> IterateAllResources()
        {
            string path = Path.Combine(Path.GetFullPath("."), BasePath);
            Debug($"Full assets path is {path}");
            if (Directory.Exists(path))
            {
                foreach (string file in Directory.EnumerateFiles(path , "*.*", SearchOption.AllDirectories))
                {
                    if(Path.GetExtension(file).Equals(".meta")) continue;

                    yield return Read(file);
                }    
            }
        }

        private static string Serialize(Resource resource)
        {
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = {new ResourceJsonConverter()}
            };

            return JsonConvert.SerializeObject(resource, options);
        }

        private static Resource Deserialize(string data, string path)
        {
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = {new ResourceJsonConverter()}
            };

            Resource resource = JsonConvert.DeserializeObject<Resource>(data, options);
            typeof(Resource).GetField("_filepath", BindingFlags.NonPublic | BindingFlags.Instance)
                .SetValue(resource, path);
            
            return resource;
        }

        private static string GetMetaPath(string path)
        {
            if (path.EndsWith(".meta"))
                return path;
            
            return path + ".meta";
        }

        private static string GetResourcePath(string path)
        {
            if (path.EndsWith(".meta"))
                return path.Remove(path.Length - 5, 5);

            return path;
        }

        private static Type GetClassType(FileType type)
        {
            switch (type)
            {
                case FileType.PlainText:
                    return typeof(TextAsset);
                case FileType.Directory:
                    return null;
                case FileType.Scene:
                    return typeof(SceneAsset);
                case FileType.Prefab:
                    break;
                case FileType.Mesh:
                    return typeof(MeshAsset);
                case FileType.Texture:
                    return typeof(TextureAsset);
                case FileType.Material:
                    return typeof(MaterialAsset);;
                case FileType.Meta:
                    return null;
                case FileType.Other:
                    return null;
                default:
                    throw new ArgumentOutOfRangeException(nameof(type), type, null);
            }

            return null;
        }

        private static FileType GetType(string path)
        {
            if (Directory.Exists(path))
                return FileType.Directory;

            FileInfo fileInfo = new FileInfo(path);

            switch (fileInfo.Extension)
            {
                case ".scene":
                    return FileType.Scene;
                case ".prefab":
                    return FileType.Prefab;
                case ".mesh":
                case ".fbx":
                case  ".obj":
                    return FileType.Mesh;
                case ".material":
                    return FileType.Material;
                case ".texture":
                case ".jpg":
                case ".png":
                    return FileType.Texture;
                case ".meta":
                    return FileType.Meta;
                case ".txt":
                    return FileType.PlainText;
            }

            return FileType.Other;
        }

        [Conditional("METADATA_DEBUG")]
        private static void Debug(string data)
        {
            Console.WriteLine(data);
        }
    }
}