#pragma once

namespace EEditorInputMode
{
	enum class Type
	{
		EditorOnlyMode,
		GameOnlyMode,
		EditorAndGameMode
	};

	inline bool IsInputSuitableForGame(Type const InMode)
	{
		return InMode == Type::EditorAndGameMode || InMode == Type::GameOnlyMode;
	}

	inline bool IsInputSuitableForEditor(Type const InMode)
	{
		return InMode == Type::EditorAndGameMode || InMode == Type::EditorOnlyMode;
	}
}

