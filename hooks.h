//
//	All hooked functions.
//
#pragma once
namespace nshooks
{
	struct Function
	{
		unsigned char* EntryPoint;
		unsigned char* SsdtHooked;
		unsigned char* ssdtOrig;
	};
}

#include "ntos.h"
namespace nshooks
{
	class hooks
	{
		static Function AllFuncs[10];
		unsigned TotalHooks;

		static
		NTSTATUS
		ZwQuerySystemInformation_Hooked(
			ULONG  SystemInformationClass,
			PVOID  SystemInformation,
			ULONG  SystemInformationLength,
			PULONG ReturnLength
		);

		static
		NTSTATUS
		nshooks::hooks::ZwTerminateProcess_Hooked(
			HANDLE ProcessHandle,
			NTSTATUS ExitStatus
		);

		bool ResolveFunction(Function *pFunction, wchar_t* FunctionName, unsigned char* HookedRoutine);
	
	public:
		bool
		Init();

		bool
		InstallHooks();

		bool
		RemoveHooks();
	};
}