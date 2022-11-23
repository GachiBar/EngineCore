using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System;
using System.Threading.Tasks;

namespace GameplayCore
{
	public static class Log
	{
		public static void PrintMessage(string Message, bool bPrintToScreen = true, bool bPrintToLog = true, float Duration = 2.0f)
		{
			if (bPrintToScreen && Duration>0.0f)
			{
				Guid guid = Guid.NewGuid();
				Task.Run(async delegate
				{
					await Task.Delay(System.TimeSpan.FromSeconds(Duration));
					Internal_RemoveLogMessage(guid.ToString());
					return;
				});
				Internal_Log(Message, bPrintToScreen, bPrintToLog, guid.ToString());
				return;
			}
			Internal_Log(Message, bPrintToScreen, bPrintToLog);
		}

		public static void PrintWarning(string Message, bool bPrintToScreen = true, bool bPrintToLog = true, float Duration = 2.0f)
		{
			Internal_LogWarning(Message, bPrintToScreen, bPrintToLog);
		}

		public static void PrintError(string Message, bool bPrintToScreen = true, bool bPrintToLog = true, float Duration = 2.0f)
		{
			Internal_LogError(Message, bPrintToScreen, bPrintToLog);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_RemoveLogMessage(string string_guid);
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_Log(string Message, bool bPrintToScreen, bool bPrintToLog,string string_guid = "");
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_LogWarning(string Message, bool bPrintToScreen, bool bPrintToLog);
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_LogError(string Message, bool bPrintToScreen, bool bPrintToLog);
	}
}
