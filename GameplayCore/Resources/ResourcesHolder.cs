using System;
using System.Collections.Generic;

namespace GameplayCore.Resources
{
    public class ResourcesHolder
    {
        private Dictionary<System.Guid, Resource> _resources;

        public ResourcesHolder()
        {
            _resources = new Dictionary<Guid, Resource>();
        }

        public bool TryGet(System.Guid guid, out Resource resource)
        {
            if (guid == System.Guid.Empty || !_resources.ContainsKey(guid))
            {
                resource = null;
                return false;
            }

            resource = _resources[guid];
            return true;
        }

        public bool Contains(System.Guid guid) => _resources.ContainsKey(guid);

        public void Set(Resource resource)
        {
            _resources[resource.Guid] = resource;
        }
    }
}