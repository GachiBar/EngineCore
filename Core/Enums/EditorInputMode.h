#pragma once
#define BIT_FLAG(x) 1<<x

namespace EEditorInputMode
{
	enum Type
	{
		EditorOnlyMode = BIT_FLAG(1),
		GameOnlyMode = BIT_FLAG(2),
		EditorAndGameMode = EditorOnlyMode | GameOnlyMode
	};

	inline bool IsInputSuitableFor(Type const InAppMode, Type const InInputMode)
	{
		return InAppMode & InInputMode;
	}
}

#undef BIT_FLAG