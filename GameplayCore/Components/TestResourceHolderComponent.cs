using GameplayCore.Resources;
using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class TestResourceHolderComponent : Component
    {
        [SerializeField] private TextAsset _asset;

        public void SetAsset(TextAsset asset) => _asset = asset;
        public System.Guid Guid => _asset.Guid;
    }
}