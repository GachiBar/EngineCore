using System;
using System.Collections.Generic;
using GameplayCore.Mathematics;
using GameplayCore.Serialization;
using GameplayCore.Editor;

namespace GameplayCore.Components
{
    public class TransformComponent : Component
    {
        [SerializeField]
        [HideInInspector]
        private TransformComponent _parent;
        private List<TransformComponent> _children;

        [SerializeField]
        [HideInInspector]
        private Quaternion _localRotation = Quaternion.Identity;

        [SerializeField]
        [InspectorName("Scale")]
        private Vector3 _localScale = Vector3.One;

        [SerializeField]
        [InspectorName("Position")]
        private Vector3 _localPosition = Vector3.Zero;

        [SerializeField] 
        [InspectorName("Rotation")] 
        [Slider(-180.0f, 180.0f)]
        private Vector3 _editorEuler;

        public int ChildrenCount => _children.Count;

        public TransformComponent Parent
        {
            get => _parent;
            set
            {
                if (value == null)
                {
                    _parent ?._children.Remove(this);
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

                _parent?._children.Remove(this);
                _parent = value;
                _parent._children.Add(this);
            }
        }

        public Quaternion LocalRotation 
        {
            get => _localRotation;
            set
            {
                _localRotation = value;
                Quaternion.ToEuler(_localRotation, out var yaw, out var pitch, out var roll);
                _editorEuler = new Vector3(pitch, yaw, roll) * 180 / MathUtil.Pi;
            }
        }

        public Vector3 LocalEuler
        {
            get
            {
                Quaternion.ToEuler(LocalRotation, out var yaw, out var pitch, out var roll);
                return new Vector3(pitch, yaw, roll) * 180 / MathUtil.Pi;
            }

            set
            {
                var euler = value * MathUtil.Pi / 180.0f;
                LocalRotation = Quaternion.RotationYawPitchRoll(euler.Y, euler.X, euler.Z);
            }
        }

        public Vector3 LocalScale 
        { 
            get => _localScale;
            set => _localScale = value; 
        }

        public Vector3 LocalPosition 
        { 
            get => _localPosition; 
            set => _localPosition = value; 
        }

        public Quaternion Rotation
        {
            get
            {
                if (Parent != null)
                {                    
                    return Parent.Rotation * LocalRotation;
                }
                else
                {
                    return LocalRotation;
                }
            }

            set
            {
                if (Parent != null)
                {
                    var quat = Parent.Rotation;
                    quat.Invert();
                    LocalRotation = quat * value;
                }
                else
                {
                    LocalRotation = value;
                }
            }
        }

        public Vector3 Euler
        {
            get
            {
                Quaternion.ToEuler(Rotation, out var yaw, out var pitch, out var roll);
                return new Vector3(pitch, yaw, roll) * 180.0f / MathUtil.Pi;
            }

            set
            {
                var euler = value * MathUtil.Pi / 180.0f;
                Rotation = Quaternion.RotationYawPitchRoll(euler.Y, euler.X, euler.Z);
            }
        }

        public Vector3 Scale
        {
            get
            {
                if (Parent != null)
                {
                    return LocalScale * Parent.Scale;
                }
                else
                {
                    return LocalScale;
                }
            }

            set
            {
                if (Parent != null)
                {
                    LocalScale = value / Parent.Scale;
                }
                else
                {
                    LocalScale = value;
                }
            }
        }

        public Vector3 Position
        {
            get
            {
                if (Parent != null)
                {
                    var transformation = Parent.ModelMatrix;
                    var localPosition = LocalPosition;
                    Vector3.Transform(ref localPosition, ref transformation, out Vector3 position);
                    return position;
                }
                else 
                {
                    return LocalPosition;
                }
            }

            set
            {
                if (Parent != null)
                {
                    var transformation = Parent.ModelMatrix;
                    transformation.Invert();
                    Vector3.Transform(ref value, ref transformation, out Vector3 position);
                    LocalPosition = position;
                }
                else
                {
                    LocalPosition = value;
                }
            }
        }

        public Matrix ModelMatrix
        {
            get
            {
                var model = Matrix.Identity;
                model *= Matrix.Scaling(LocalScale);
                model *= Matrix.RotationQuaternion(LocalRotation);
                model *= Matrix.Translation(LocalPosition);               

                if (Parent != null)
                {
                    model *= Parent.ModelMatrix;
                }

                return model;
            }
        }

        public Vector3 Right => Rotate(Vector3.Right, Rotation);
        public Vector3 Left => Rotate(Vector3.Left, Rotation);
        public Vector3 Up => Rotate(Vector3.Up, Rotation);
        public Vector3 Down => Rotate(Vector3.Down, Rotation);
        public Vector3 Forward => Rotate(Vector3.ForwardRH, Rotation);
        public Vector3 Backward => Rotate(Vector3.BackwardRH, Rotation);

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

        public TransformComponent GetChild(int index)
        {
            return _children[index];
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

        internal override void Invalidate(string fieldName)
        {
            if (fieldName == nameof(_editorEuler))
            {
                var euler = _editorEuler * MathUtil.Pi / 180.0f;
                _localRotation = Quaternion.RotationYawPitchRoll(euler.Y, euler.X, euler.Z);
            }
        }

        private Vector3 Rotate(Vector3 vector, Quaternion rotation)
        {
            Vector3.Transform(ref vector, ref rotation, out var result);
            return result;
        }
    }
}
