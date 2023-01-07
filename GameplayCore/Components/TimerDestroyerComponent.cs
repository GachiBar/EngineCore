using GameplayCore.Serialization;

namespace GameplayCore.Components
{
    public class TimerDestroyerComponent : Component
    {
        [SerializeField]
        private float _timer;

        public float Timer
        {
            get => _timer;
            set => _timer = value;
        }

        public override void Update()
        {
            _timer -= Time.DeltaTime;

            if (_timer <= 0)
            {
                GameObject.Scene.DeleteGameObject(GameObject);
            }
        }
    }
}
