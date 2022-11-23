using System;
using System.IO;
using GameplayCore.Serialization;
using Newtonsoft.Json;

namespace GameplayCore.Resources
{
    public class MetadataReader
    {
        public static Resource Create(string path)
        {
            FileType assetType = GetType(path);
            Type classType = GetClassType(assetType);
            if(classType == null)
                return null;
            
            Resource resource = Activator.CreateInstance(classType) as Resource;
            Write(resource, path);

            return resource;
        }

        public static Resource Read(string path)
        {
            if (!File.Exists(path))
                return Create(path);
            
            FileStream file = File.OpenRead(path);
            StreamReader writer = new StreamReader(file);
            Resource resource = Deserialize(writer.ReadToEnd());
            file.Close();

            return resource;
        }

        public static void Write(Resource resource, string path)
        {
            FileStream file = File.OpenWrite(path);
            StreamWriter writer = new StreamWriter(file);
            writer.Write(Serialize(resource));
            file.Close();
        }

        public static Resource GetByGuid()
        {
            // Find by _guid in files
            
            throw new NotImplementedException();
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

        private static Resource Deserialize(string data)
        {
            JsonSerializerSettings options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = {new ResourceJsonConverter()}
            };

            return JsonConvert.DeserializeObject(data) as Resource;
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
                    return FileType.Texture;
                case ".meta":
                    return FileType.Meta;
                case ".txt":
                    return FileType.PlainText;
            }

            return FileType.Other;
        }
    }
}