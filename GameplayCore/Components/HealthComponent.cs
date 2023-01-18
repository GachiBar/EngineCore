using GameplayCore.Editor;
using GameplayCore.Mathematics;
using GameplayCore.Serialization;
using System;

namespace GameplayCore.Components
{
    public class HealthComponent : Component
    {
        [SerializeField]
        [InspectorName("MaxHealth")]
        private float _maxHealth = 100.0f;
        [SerializeField]
        [InspectorName("Health")]
        private float _healt = 100.0f;
        [SerializeField]
        [InspectorName("RegenerationSpeed")]
        private float _regenerationSpeed = 0.0f;

        public float MaxHealth
        {
            get => _maxHealth;
            set => _maxHealth = Math.Max(0, value);
        }

        public float Health => _healt;

        public event Action<HealthComponent> Died;

        public void Damage(float damage)
        {
            _healt = Math.Max(0, _healt - damage);

            if (_healt <= 0)
            {
                Died?.Invoke(this);
            }
        }

        public override void Initialize()
        {
            _healt = _maxHealth;
        }

        public override void Update()
        {
            if (_healt > 0)
            {
                _healt += _regenerationSpeed * Time.DeltaTime;
                _healt = MathUtil.Clamp(_healt, 0, _maxHealth);
            }            
        }
    }
}
