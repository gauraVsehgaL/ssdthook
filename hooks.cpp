#include <ntddk.h>
#include "hooks.h"
#include "ntos.h"
#include "repstry.h"

hookrepstry HookRpstry;
nshooks::Function nshooks::hooks::AllFuncs[10];

NTSTATUS
nshooks::hooks::ZwQuerySystemInformation_Hooked(
	ULONG SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
)
{
	Function function = nshooks::hooks::AllFuncs[0];
	auto OrigFunc = reinterpret_cast<ntos::pfnZwQuerySystemInformation>(function.ssdtOrig);
	return OrigFunc(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
}

NTSTATUS
nshooks::hooks::ZwTerminateProcess_Hooked(
	HANDLE ProcessHandle,
	NTSTATUS ExitStatus
)
{
	DbgPrint("ZwTerminateProcess called by 0x%08x \n", PsGetCurrentProcessId());

	Function function = nshooks::hooks::AllFuncs[1];
	auto OrigFunc = reinterpret_cast<ntos::pfnZwTerminateProcess>(function.ssdtOrig);
	return OrigFunc(ProcessHandle, ExitStatus);
}

bool
nshooks::hooks::Init()
{
	if (!HookRpstry.Init())
	{
		return false;
	}

	TotalHooks = 0;

	ResolveFunction(&AllFuncs[0], L"ZwQuerySystemInformation", reinterpret_cast<PUCHAR>(ZwQuerySystemInformation_Hooked));
	TotalHooks++;
	ResolveFunction(&AllFuncs[1], L"ZwTerminateProcess", reinterpret_cast<PUCHAR>(ZwTerminateProcess_Hooked));
	TotalHooks++;
	
	return true;
}

bool
nshooks::hooks::ResolveFunction(Function *pFunction, wchar_t* FunctionName, unsigned char* HookedRoutine)
{
	pFunction->EntryPoint = (unsigned char*)ntos::ntos::ResolveFunctionPointers(FunctionName);
	pFunction->SsdtHooked = HookedRoutine;
	pFunction->ssdtOrig = nullptr;
	return true;
}

bool
nshooks::hooks::InstallHooks()
{	
	auto TotalHooksInstalled = HookRpstry.InstallHooks(AllFuncs, TotalHooks);
	TotalHooksInstalled;
	return true;
}

bool
nshooks::hooks::RemoveHooks()
{
	HookRpstry.RemoveHooks(AllFuncs, TotalHooks);
	return true;
}