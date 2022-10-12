namespace GameplayCore.Components
{
    public abstract class Component
    {
        public GameObject GameObject { get; internal set; }

        public virtual void Initialize() { }
        public virtual void FixedUpdate() { }
        public virtual void Update() { }
        public virtual void Render() { }
        public virtual void Terminate() { }
    }
}
