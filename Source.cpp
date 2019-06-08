#include <wdm.h>
#include "repstry.h"

nshooks::hooks Hooking;

void
Unload(PDRIVER_OBJECT);

extern "C"
NTSTATUS
DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING)
{
	Hooking.Init();
	if (!Hooking.Init())
	{
		DbgPrint("Hooking::Init() Failed");
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	if (!Hooking.InstallHooks())
	{
		DbgPrint("InstallHooks() Failed ");
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	DriverObject->DriverUnload = Unload;

	return STATUS_SUCCESS;
}

void
Unload(PDRIVER_OBJECT)
{
	Hooking.RemoveHooks();
}
