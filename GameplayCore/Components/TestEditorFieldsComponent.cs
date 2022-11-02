using GameplayCore.Mathematics;

namespace GameplayCore.Components
{
    public class TestEditorFieldsComponent : Component
    {
        private float _floatValue;
        private double _doubleValue;
        private bool _boolValue;
        private sbyte _byteValue;
        private short _shortValue;
        private int _intValue;
        private long _longValue;
        private byte _ubyteValue;
        private ushort _ushortValue;
        private uint _uintValue;
        private ulong _ulongValue;
        private Vector2 _vector2Value;
        private Vector3 _vector3Value;
        private Vector4 _vector4Value;
        private string _stringValue = "TestString";
        private GameObject _gameObjectValue;

        public float FloatProperty 
        {
            get => _floatValue;
            set
            {
                System.Console.WriteLine($"{nameof(FloatProperty)} = {value}");
                _floatValue = value;
            }
        }

        public double DoubleProperty 
        {
            get => _doubleValue;
            set
            {
                System.Console.WriteLine($"{nameof(DoubleProperty)} = {value}");
                _doubleValue = value;
            }
        }

        public bool BoolProperty 
        { 
            get => _boolValue;
            set
            {
                System.Console.WriteLine($"{nameof(BoolProperty)} = {value}");
                _boolValue = value;
            }
        }

        public sbyte ByteProperty 
        { 
            get => _byteValue;
            set 
            {
                System.Console.WriteLine($"{nameof(ByteProperty)} = {value}");
                _byteValue = value;
            }
        }

        public short ShortProperty 
        { 
            get => _shortValue;
            set
            {
                System.Console.WriteLine($"{nameof(ShortProperty)} = {value}");
                _shortValue = value;
            }
        }

        public int IntProperty 
        { 
            get => _intValue;
            set
            {
                System.Console.WriteLine($"{nameof(IntProperty)} = {value}");
                _intValue = value;
            }
        }

        public long LongProperty 
        { 
            get => _longValue;
            set
            {
                System.Console.WriteLine($"{nameof(LongProperty)} = {value}");
                _longValue = value;
            }
        }

        public byte UByteProperty 
        { 
            get => _ubyteValue;
            set
            {
                System.Console.WriteLine($"{nameof(UByteProperty)} = {value}");
                _ubyteValue = value;
            }
        }

        public ushort UShortProperty 
        { 
            get => _ushortValue;
            set
            {
                System.Console.WriteLine($"{nameof(UShortProperty)} = {value}");
                _ushortValue = value;
            }
        }

        public uint UIntProperty 
        { 
            get => _uintValue;
            set
            {
                System.Console.WriteLine($"{nameof(UIntProperty)} = {value}");
                _uintValue = value;
            }
        }

        public ulong ULongProperty 
        { 
            get => _ulongValue;
            set
            {
                System.Console.WriteLine($"{nameof(ULongProperty)} = {value}");
                _ulongValue = value;
            }
        }

        public Vector2 Vector2Property 
        { 
            get => _vector2Value;
            set
            {
                System.Console.WriteLine($"{nameof(Vector2Property)} = {value}");
                _vector2Value = value;
            }
        }

        public Vector3 Vector3Property
        {
            get => _vector3Value;
            set
            {
                System.Console.WriteLine($"{nameof(Vector3Property)} = {value}");
                _vector3Value = value;
            }
        }


        public Vector4 Vector4Property
        {
            get => _vector4Value;
            set
            {
                System.Console.WriteLine($"{nameof(Vector4Property)} = {value}");
                _vector4Value = value;
            }
        }

        public string StringProperty
        {
            get => _stringValue;
            set
            {
                System.Console.WriteLine($"{nameof(StringProperty)} = {value}");
                _stringValue = value;
            }
        }

        public GameObject GameObjectProperty 
        { 
            get => _gameObjectValue;
            set
            {
                System.Console.WriteLine($"{nameof(GameObjectProperty)} = {value}");
                _gameObjectValue = value;
            }
        }
    }
}
