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

            Console.WriteLine(resource);
            return resource;
        }

        public static Resource Read(string path)
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

        public static Resource GetByGuid(System.Guid guid)
        {
            if (guid.Equals(System.Guid.Empty)) 
                return null;

            foreach (var resource in IterateAllResources())
            {
                if (resource != null && resource.Guid.Equals(guid))
                    return resource;
            }

            return null;
        }

        private static IEnumerable<Resource> IterateAllResources()
        {
            // TODO: Add ResourceHolder : Dictionary<System.Guid, Resource>
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
                    break;
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