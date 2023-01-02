using GameplayCore.Mathematics;
using System.Collections.Generic;

namespace GameplayCore.AI
{
    public class State
    {
        private Dictionary<string, bool> _keyToBoolValues = new Dictionary<string, bool>();
        private Dictionary<string, int> _keyToIntValues = new Dictionary<string, int>();
        private Dictionary<string, float> _keyToFloatValues = new Dictionary<string, float>();
        private Dictionary<string, Vector2> _keyToVector2Values = new Dictionary<string, Vector2>();
        private Dictionary<string, Vector3> _keyToVector3Values = new Dictionary<string, Vector3>();
        private Dictionary<string, GameObject> _keyToGameObjects = new Dictionary<string, GameObject>();

        public bool GetBoolValue(string key)
        {
            return _keyToBoolValues[key];
        }

        public void SetBoolValue(string key, bool value)
        {
            _keyToBoolValues[key] = value;
        }

        public bool TryGetBoolValue(string key, out bool value)
        {
            return _keyToBoolValues.TryGetValue(key, out value);
        }

        public int GetIntValue(string key)
        {
            return _keyToIntValues[key];
        }

        public void SetIntValue(string key, int value)
        {
            _keyToIntValues[key] = value;
        }

        public bool TryGetIntValue(string key, out int value)
        {
            return _keyToIntValues.TryGetValue(key, out value);
        }

        public float GetFloatValue(string key)
        {
            return _keyToFloatValues[key];
        }

        public void SetFloatValue(string key, float value)
        {
            _keyToFloatValues[key] = value;
        }

        public bool TryGetFloatValue(string key, float value)
        {
            return _keyToFloatValues.TryGetValue(key, out value);
        }

        public Vector2 GetVector2Value(string key)
        {
            return _keyToVector2Values[key];
        }

        public void SetVector2Value(string key, Vector2 value)
        {
            _keyToVector2Values[key] = value;
        }

        public bool TryGetVector2Value(string key, out Vector2 value)
        {
            return _keyToVector2Values.TryGetValue(key, out value);
        }

        public Vector3 GetVector3Value(string key)
        {
            return _keyToVector3Values[key];
        }

        public void SetVector3Value(string key, Vector3 value)
        {
            _keyToVector3Values[key] = value;
        }

        public bool TryGetVector3Value(string key, out Vector3 value)
        {
            return _keyToVector3Values.TryGetValue(key, out value);
        }

        public GameObject GetGameObjectValue(string key)
        {
            return _keyToGameObjects[key];
        }

        public void SetGameObjectValue(string key, GameObject gameObject)
        {
            _keyToGameObjects[key] = gameObject;
        }

        public bool TryGetGameObjectValue(string key, out GameObject gameObject)
        {
            return _keyToGameObjects.TryGetValue(key, out gameObject);
        }

        public bool IsSame(State other)
        {
            return IsValuesEqual(_keyToBoolValues, other._keyToBoolValues)
                && IsValuesEqual(_keyToIntValues, other._keyToIntValues)
                && IsValuesEqual(_keyToFloatValues, other._keyToFloatValues)
                && IsValuesEqual(_keyToVector2Values, other._keyToVector2Values)
                && IsValuesEqual(_keyToVector3Values, other._keyToVector3Values)
                && IsValuesEqual(_keyToGameObjects, other._keyToGameObjects);
        }

        public State Copy()
        {
            var copy = new State();
            copy._keyToBoolValues = new Dictionary<string, bool>(_keyToBoolValues);
            copy._keyToIntValues = new Dictionary<string, int>(_keyToIntValues);
            copy._keyToFloatValues = new Dictionary<string, float>(_keyToFloatValues);
            copy._keyToVector2Values = new Dictionary<string, Vector2>(_keyToVector2Values);
            copy._keyToVector3Values = new Dictionary<string, Vector3>(_keyToVector3Values);
            copy._keyToGameObjects = new Dictionary<string, GameObject>(_keyToGameObjects);
            return copy;
        }

        private static bool IsValuesEqual<TValue>(
            Dictionary<string, TValue> thisValues, 
            Dictionary<string, TValue> otherValues)
        {
            foreach (var kvp in thisValues)
            {
                if (otherValues.TryGetValue(kvp.Key, out var otherValue))
                {
                    if (kvp.Value == null || otherValue == null)
                    {
                        return kvp.Value == null && otherValue == null;
                    }
                    if (!kvp.Value.Equals(otherValue))
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }

            return true;
        }
    }
}
