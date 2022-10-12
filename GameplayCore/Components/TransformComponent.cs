using System;
using System.Collections.Generic;
using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class TransformComponent : Component
    {
        private TransformComponent _parent;
        private List<TransformComponent> _children;

        public int ChildrenCount => _children.Count;

        public TransformComponent Parent
        {
            get => _parent;
            set
            {
                if (value == null)
                {
                    Parent?._children.Remove(this);
                    _parent = null;
                    return;
                }

                if (_parent == this)
                {
                    throw new ArgumentException("Game object can't be parent of itself.");
                }

                if (value.IsChildOf(this))
                {
                    throw new ArgumentException("Parent doesn't have to be a child.");
                }

                _parent = value;
                _parent._children.Add(this);
            }
        }

        public Quaternion LocalRotation { get; set; } = Quaternion.Identity;

        public Vector3 LocalEuler
        {
            get => LocalRotation.EulerAngles;
            set => LocalRotation = Quaternion.Euler(value);
        }

        public Vector3 LocalScale { get; set; } = Vector3.One;

        public Vector3 LocalPosition { get; set; } = Vector3.Zero;

        public Quaternion Rotation
        {
            get => Parent != null
                ? LocalRotation * Parent.Rotation
                : LocalRotation;
            set => LocalRotation = Parent != null 
                ? Quaternion.Inverse(Parent.Rotation) * value : 
                value;
        }

        public Vector3 Euler
        {
            get => Rotation.EulerAngles;
            set => Rotation = Quaternion.Euler(value);
        }

        public Vector3 Scale
        {
            get => Parent != null 
                ? LocalScale * Parent.Scale 
                : LocalScale;
            set => LocalScale = Parent != null 
                ? value / Parent.Scale 
                : value;
        }

        public Vector3 Position
        {
            get => Parent != null
                ? LocalPosition * Parent.ModelMatrix
                : LocalPosition;
            set => LocalPosition = Parent != null
                ? value * Parent.ModelMatrix.Invert()
                : value;
        }

        public Matrix ModelMatrix
        {
            get
            {
                var model = Matrix.ModelMatrix(LocalScale, LocalRotation, LocalPosition);
                
                if (Parent != null)
                {
                    model *= Parent.ModelMatrix;
                }

                return model;
            }
        }

        public TransformComponent()
        {
            Parent = null;
            _children = new List<TransformComponent>();
        }

        public override void Terminate()
        {
            Parent = null;

            while (_children.Count > 0)
            {
                _children[0].Parent = null;
            }
        }

        public bool IsChildOf(TransformComponent transform)
        {
            TransformComponent temp = this;

            do
            {
                if (temp.Parent == null)
                {
                    return false;
                }

                if (temp.Parent == transform)
                {
                    return true;
                }

                temp = temp.Parent;
            } 
            while (true);
        }

        public TransformComponent this[int index]
        {
            get => _children[index];
        }
    }
}
