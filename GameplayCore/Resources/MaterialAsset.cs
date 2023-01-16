using System;
using System.IO;
using GameplayCore.EngineApi;
using GameplayCore.Serialization;
using Newtonsoft.Json;

namespace GameplayCore.Resources
{
    public class MaterialAsset : Resource, ILoadable
    {
        private Material _material;
        private ulong _id = 0;
        
        public ulong Id => _id;

        // Creates or changes material data by id to c++ side
        public MaterialAsset(string filepath, Guid? guid = null) : base(filepath, guid)
        {
        }

        public void Commit()
        {
            // Set
            if(_material == null)
                LoadData();
            
            RenderApi.CommitMaterial(_id, new TransferMaterial(_material));
        }

        // Gets data from c++ side

        public void Pull()
        {
            RenderApi.PullMaterial(_id);
            // Why would you pull material from c++ side
            // TODO: Do something with dis or just remove method
        }
        
        public void LoadData()
        {
            Console.WriteLine($"Loading material data of {FilePath}!");
            string data = String.Empty;

            using (StreamReader reader = new StreamReader(FilePath))
            {
                data = reader.ReadToEnd();
            }

            _material = JsonConvert.DeserializeObject<Material>(data, CreateSerializeOptions());
        }

        public void SaveData()
        {
            Console.WriteLine($"Saving material data of {FilePath}!");
            Save(_material);

            // Invalidate meshes at scene
            if(Id != 0)
                Commit();
        }

        public void Create()
        {
            _material = new Material();
            Save(_material);
        }

        private void Save(Material material)
        {
            // Serialize -> string json
            string json = JsonConvert.SerializeObject(material, CreateSerializeOptions());
            File.WriteAllText(FilePath, json);
        }
        
        public void Load()
        {
            if (_id != 0)
            {
                Console.WriteLine($"Avoid loading material with id {_id}");
                return;
            }

            Console.WriteLine($"Loading new material with id {_id}");
            ulong id = RenderApi.CalculateFreeMaterialId();
            _id = id;

            // TODO: If _material == null -> Load from dist
            LoadData();
            
            // TODO: Texture load if there is a texture
            if(_material.diffuseTexture != null)
                _material.diffuseTexture.Load();
            
            if(_material.normalsTexture != null)
                _material.normalsTexture.Load();
            
            if(_material.roughnessTexture != null)
                _material.roughnessTexture.Load();
            
            if(_material.metallicTexture != null)
                _material.metallicTexture.Load();

            Commit();
        }

        private static JsonSerializerSettings CreateSerializeOptions()
        {
            JsonSerializerSettings _options = new JsonSerializerSettings()
            {
                Formatting = Formatting.Indented,
                Converters = {new ResourceGuidJsonConverter()}
            };
            return _options;
        }
        
        
    }
}