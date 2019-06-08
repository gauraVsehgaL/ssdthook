#include <ntddk.h>
#include "ntos.h"
#include "hooks.h"

class hookrepstry
{
private:
	
	ntos::SystemServiceTable *Ssdt;

	void
	DisableWPinRegister()
	{
		_asm
		{
			push ecx;
			mov ecx, cr0;
			and ecx, 0xFFFEFFFF;
			mov cr0, ecx;
			pop ecx;
		}
	}

	void
	EnableWPinRegister()
	{
		_asm
		{
			push ecx;
			mov ecx, cr0;
			or ecx, 0x00010000;
			mov cr0, ecx;
			pop ecx;
		}
	}

	unsigned char*
	Hook(unsigned char *FunctionEntry, unsigned char *HookedFunction)
	{
		auto ssdt = Ssdt->ServiceTable;
		auto index = *(reinterpret_cast<int*>(FunctionEntry + 1));

		auto Target = reinterpret_cast<unsigned char*>(&ssdt[index]);

		return reinterpret_cast<unsigned char*>(InterlockedExchange(reinterpret_cast<volatile LONG*>(Target), reinterpret_cast<LONG>(HookedFunction)));
	}

public:
	bool
	Init()
	{
		Ssdt = ntos::ntos::GetSsdtTable();
		return Ssdt != nullptr;
	}

	unsigned int InstallHooks(nshooks::Function *AllFunctions, int Count)
	{
		unsigned int TotalHooksInstalled = 0;
		DisableWPinRegister();

		for (auto i = 0; i < Count; i++)
		{
			if (!AllFunctions[i].EntryPoint)
				continue;

			auto Orig = Hook(AllFunctions[i].EntryPoint, AllFunctions[i].SsdtHooked);
			
			AllFunctions[i].ssdtOrig = Orig;
			
			TotalHooksInstalled++;
		}

		EnableWPinRegister();

		return TotalHooksInstalled;
	}

	unsigned int RemoveHooks(nshooks::Function *AllFunctions, int Count)
	{
		unsigned int TotalHooksInstalled = 0;
		DisableWPinRegister();

		for (auto i = 0; i < Count; i++)
		{
			if (!AllFunctions[i].EntryPoint)
				continue;

			Hook(AllFunctions[i].EntryPoint, AllFunctions[i].ssdtOrig);

			TotalHooksInstalled++;
		}

		EnableWPinRegister();

		return TotalHooksInstalled;
	}
};