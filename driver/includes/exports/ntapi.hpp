#pragma once

extern "C" {
    NTKERNELAPI NTSTATUS NTAPI MmCopyVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAddress, PEPROCESS TargetProcess, PVOID TargetAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize);
    NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(IN HANDLE ProcessId, OUT PEPROCESS* Process);
	NTKERNELAPI PVOID PsGetProcessSectionBaseAddress(IN PEPROCESS Process);
    NTKERNELAPI PPEB PsGetProcessPeb(IN PEPROCESS Process);
    NTKERNELAPI PVOID NTAPI PsGetProcessWow64Process(IN PEPROCESS Process);
};