#include "headers.hpp"

ed::driver_interface interface;
NTSTATUS CustomDriverEntry(_In_ PDRIVER_OBJECT kdmapperParam1, _In_ PUNICODE_STRING kdmapperParam2) {
	UNREFERENCED_PARAMETER(kdmapperParam1);
	UNREFERENCED_PARAMETER(kdmapperParam2);
	
	if(!NT_SUCCESS(KsInitialize())) return STATUS_FAILED_DRIVER_ENTRY;
	return interface.init();
};