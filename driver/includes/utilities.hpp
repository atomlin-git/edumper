#pragma once

#include "exports/peb.hpp"
#include "exports/ntapi.hpp"

#define sector_size 3

namespace ed {
    static unsigned char* process(unsigned char* data, unsigned long long length) {
        if(!data) return 0;

        for(unsigned int i = 0; i < length; i++) data[i] ^= 0xC9;
        unsigned int sector_count = length / sector_size;
        unsigned char sector[sector_size];
        if(!((sector_count / length) % 2)) sector_count--;
        
        for(unsigned int i = 0; i < (sector_count * sector_size); i += sector_size * 2) {
            if(i + sector_size > length) continue;
            memcpy(&sector[0], &data[i], sector_size);
            
            memcpy(&data[i], &data[i + sector_size], sector_size);
            memcpy(&data[i + sector_size], &sector[0], sector_size);
        };
        
        return data;
    };
    
    static NTSTATUS read_virtual_memory(unsigned int process_id, unsigned char* buffer, unsigned long long address, unsigned long long size) {
        PEPROCESS process = 0;
        if(!buffer) return STATUS_INVALID_PARAMETER_2;
        if(!NT_SUCCESS(PsLookupProcessByProcessId(HANDLE(process_id), &process))) return STATUS_INVALID_PARAMETER_1;
		unsigned long long s = 0;
        MmCopyVirtualMemory(process, PVOID(address), PsGetCurrentProcess(), PVOID(buffer), size, KernelMode, &s);
        ObDereferenceObject(process);
        return STATUS_SUCCESS;
    };

    static NTSTATUS get_process_module_info(unsigned int process_id, const char* module, unsigned long long& base, unsigned long long& size, bool wow64 = false) {
        PEPROCESS process = 0;
        if(!NT_SUCCESS(PsLookupProcessByProcessId(HANDLE(process_id), &process))) return STATUS_INVALID_PARAMETER_1;
        
        KAPC_STATE state;
        KeStackAttachProcess((PRKPROCESS)process, &state);
        
        ANSI_STRING module_ansi;
        UNICODE_STRING module_unicode;
        RtlInitAnsiString(&module_ansi, module);
        if(!NT_SUCCESS(RtlAnsiStringToUnicodeString(&module_unicode, &module_ansi, TRUE))) return STATUS_INVALID_PARAMETER_2;

        if(!wow64) {
            PPEB32 peb32 = (PPEB32)PsGetProcessWow64Process(process);
            for (PLIST_ENTRY32 list = (PLIST_ENTRY32)((PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList.Flink; list != &((PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList; list = (PLIST_ENTRY32)list->Flink) {
                PLDR_DATA_TABLE_ENTRY32 entry = CONTAINING_RECORD(list, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);
                UNICODE_STRING current_module;
                
                RtlCreateUnicodeString(&current_module, (PWCH)entry->BaseDllName.Buffer);
                if (RtlCompareUnicodeString(&current_module, &module_unicode, TRUE) == 0) {
                    base = entry->DllBase;
                    size = entry->SizeOfImage;
                };
            };
        } else {
            PPEB peb = (PPEB)PsGetProcessPeb(process);
            for (PLIST_ENTRY list = (PLIST_ENTRY)((PPEB_LDR_DATA)peb->Ldr)->InLoadOrderModuleList.Flink; list != &((PPEB_LDR_DATA)peb->Ldr)->InLoadOrderModuleList; list = (PLIST_ENTRY)list->Flink) {
                PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(list, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
                UNICODE_STRING current_module;
                
                RtlCreateUnicodeString(&current_module, entry->BaseDllName.Buffer);
                if(RtlCompareUnicodeString(&current_module, &module_unicode, TRUE) == 0) {
                    base = (unsigned long long)entry->DllBase;
                    size = (unsigned long long)entry->SizeOfImage;
                };
            };
        };

        RtlFreeUnicodeString(&module_unicode);
        KeUnstackDetachProcess(&state);
        ObDereferenceObject(process);
        return STATUS_SUCCESS;
    };
};