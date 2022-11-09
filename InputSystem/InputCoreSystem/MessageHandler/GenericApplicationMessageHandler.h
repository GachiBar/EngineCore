#pragma once

#include <memory>
#include <queue>

#include "SimpleMath.h"
#include "../InputEvent/Events.h"
#include "../../../Core/Interfaces/IApplication.h"
#include "../../../Core/Windows/GenericWindow.h"


struct FInputEvent;

enum class EWindowActivation : uint8
{
	Activate,
	ActivateByMouse,
	Deactivate
};

namespace EWindowZone
{
	/**
	 * The Window Zone is the window area we are currently over to send back to the operating system
	 * for operating system compliance.
	 */
	enum Type
	{
		NotInWindow			= 0,
		TopLeftBorder		= 1,
		TopBorder			= 2,
		TopRightBorder		= 3,
		LeftBorder			= 4,
		ClientArea			= 5,
		RightBorder			= 6,
		BottomLeftBorder	= 7,
		BottomBorder		= 8,
		BottomRightBorder	= 9,
		TitleBar			= 10,
		MinimizeButton		= 11,
		MaximizeButton		= 12,
		CloseButton			= 13,
		SysMenu				= 14,

		/** No zone specified */
		Unspecified	= 0,
	};
}

namespace EMouseButtons
{
	enum Type
	{
		Left = 0,
		Middle,
		Right,
		Thumb01,
		Thumb02,

		Invalid,
	};
}

/** Interface that defines how to handle interaction with a user via hardware input and output */
class FGenericApplicationMessageHandler
{
public:

	virtual ~FGenericApplicationMessageHandler()
	{
	}

	virtual bool OnKeyChar(const wchar_t Character, const bool IsRepeat)
	{
		return false;
	}

	virtual bool OnKeyDown( const int32 KeyCode, const uint32 CharacterCode, const bool IsRepeat, const bool IsModified)
	{
		return false;
	}

	virtual bool OnKeyUp( const int32 KeyCode, const uint32 CharacterCode, const bool IsRepeat , const bool IsModified)
	{
		return false;
	}

	virtual void OnInputLanguageChanged()
	{
	}

	virtual bool OnMouseDown( const std::shared_ptr< FGenericWindow >& Window, const EMouseButtons::Type Button )
	{
		return false;
	}

	virtual bool OnMouseDown( const std::shared_ptr< FGenericWindow >& Window, const EMouseButtons::Type Button, const FVector2D CursorPos )
	{
		return false;
	}

	virtual bool OnMouseUp( const EMouseButtons::Type Button )
	{
		return false;
	}

	virtual bool OnMouseUp( const EMouseButtons::Type Button, const FVector2D CursorPos )
	{
		return false;
	}

	virtual bool OnMouseDoubleClick( const std::shared_ptr< FGenericWindow >& Window, const EMouseButtons::Type Button )
	{
		return false;
	}

	virtual bool OnMouseDoubleClick( const std::shared_ptr< FGenericWindow >& Window, const EMouseButtons::Type Button, const FVector2D CursorPos )
	{
		return false;
	}

	virtual bool OnMouseWheel( const float Delta )
	{
		return false;
	}

	virtual bool OnMouseWheel( const float Delta, const FVector2D CursorPos )
	{
		return false;
	}

	virtual bool OnMouseMove()
	{
		return false;
	}

	virtual bool OnRawMouseMove( const int32 X, const int32 Y )
	{
		return false;
	}

	/** 
	 * Return true if this message handler expects FPlatformUserIds. This base class will convert both directions.
	 * As part of a larger fixup to allow mapping of multiple input devices to the same player, 
	 * physical device id will be passed as part of the InputScope above and used to compute a logical input user.
	 */
	virtual bool ShouldUsePlatformUserId() const
	{
		return false;
	}

	virtual void OnSizeChanged(const std::shared_ptr< FGenericWindow >& PlatformWindow, const int32 Width, const int32 Height)
	{
		
	}

	virtual void OnResizingWindow(const std::shared_ptr< FGenericWindow >& Window, bool bWasMinimized)
	{

	}

	virtual void OnMovedWindow( const std::shared_ptr< FGenericWindow >& Window, const int32 X, const int32 Y )
	{

	}

	virtual bool OnApplicationActivationChanged( const bool InAppActivated)
	{
		return false;
	}

	virtual void OnWindowClose( const std::shared_ptr< FGenericWindow >& Window )
	{

	}

	virtual void SetCursorPos(const FVector2D& MouseCoordinate)
	{

	}

	std::queue< std::shared_ptr<FInputEvent>> const & GetInputStack()
	{
		return InputStack;
	}

	bool ReadInputEvent(std::shared_ptr<FInputEvent>& Event_out)
	{
		if(InputStack.size())
		{
			Event_out = InputStack.front();
			InputStack.pop();
			return true;
		}
		Event_out = nullptr;
		return false;
	}

protected:
	std::queue<std::shared_ptr<FInputEvent>> InputStack;
};

