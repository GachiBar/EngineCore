using GameplayCore.Editor;
using GameplayCore.Mathematics;
using ImGuiNET;
using System;
using System.Text;

namespace GameplayCore.Components
{
    public class TestEditorFieldsComponent : Component
    {
        public float FloatValue;
        public double DoubleValue;
        public bool BoolValue;
        public sbyte ByteValue;
        public short ShortValue;
        public int IntValue;
        public long LongValue;
        public byte UbyteValue;
        public ushort UshortValue;
        public uint UintValue;
        public ulong UlongValue;
        [Slider(-180.0f, 180.0f)]
        public Vector2 Vector2Value;
        [Slider(-180.0f, 180.0f)]
        public Vector3 Vector3Value;
        [Slider(-180.0f, 180.0f)]
        public Vector4 Vector4Value;
        public string StringValue = "TestString";
        public GameObject GameObjectValue;


		public override void RenderGUI()
        {
            Log.PrintMessage("Here!!",Duration:0.0f);
            ImGui.BeginChild("This is from C#");

			Log.PrintMessage("Here!!1", Duration: 0.0f);
			ImGui.LabelText("Ochepyatka", "");
			//ImGui.LabelText("Ochepyatka", "");
			//ImGui.LabelText("Ochepyatka", "");
			Log.PrintMessage("Here!!2", Duration: 0.0f);
			ImGui.EndChild();
			Log.PrintMessage("Here!!3", Duration: 0.0f);
		}
    }
}
