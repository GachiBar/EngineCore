using GameplayCore.Serialization;

namespace GameplayCore.Resources
{
    public class MaterialAsset
    {
        [SerializeField] private Material _material;
        private ulong _id;
        
        // Creates or changes material data by id to c++ side
        public void Commit()
        {
            // Set
        }

        // Gets data from c++ side
        public void Pull()
        {
            
        }

        // Sets id for render
        public void Set()
        {
            // Just calls extern method
        }
    }
}