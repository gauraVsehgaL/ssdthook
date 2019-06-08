//
//	undocumented structures and functions.
//
#pragma once
namespace ntos
{
	typedef struct _SystemServiceTable {
		UINT32* 	ServiceTable;
		UINT32* 	CounterTable;
		UINT32		ServiceLimit;
		UINT32*     ArgumentTable;
	} SystemServiceTable;

	typedef
	NTSTATUS(*pfnZwQuerySystemInformation)(
		ULONG  SystemInformationClass,
		PVOID  SystemInformation,
		ULONG  SystemInformationLength,
		PULONG ReturnLength
		);

	typedef
	NTSTATUS (*pfnZwTerminateProcess)(
		HANDLE ProcessHandle,
		NTSTATUS ExitStatus
		);
};


namespace ntos
{
	class ntos
	{
	public:
		pfnZwQuerySystemInformation ZwQuerySystemInformation = nullptr;
		pfnZwTerminateProcess ZwTerminateProcess = nullptr;

		SystemServiceTable *KeServiceDescriptorTable = nullptr;

		static
		SystemServiceTable*
		GetSsdtTable()
		{
			UNICODE_STRING usKeServiceDescriptorTable;
			RtlInitUnicodeString(&usKeServiceDescriptorTable, L"KeServiceDescriptorTable");
			return reinterpret_cast<SystemServiceTable*>(MmGetSystemRoutineAddress(&usKeServiceDescriptorTable));
		}

		static
		void*
		ResolveFunctionPointers(wchar_t * FuncName)
		{
			UNICODE_STRING us;

			RtlInitUnicodeString(&us, FuncName);
			return MmGetSystemRoutineAddress(&us);
		}
	};
}