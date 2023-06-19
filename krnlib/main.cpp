#include <ntifs.h>
#include <krnlib/initializer_list.hpp>
#include <krnlib/functional.hpp>

void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "п╤ть!\n"));
	return;
}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pRegistryPath);

	pDriverObject->DriverUnload = DriverUnload;

	krnlib::initializer_list<int> jjj{1, 3, 4};
	krnlib::function<void()> aas = [&]() {
		for (auto& i : jjj)
		{
			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "%d\n", i));
		}
	};
	aas();
	return STATUS_SUCCESS;
}
