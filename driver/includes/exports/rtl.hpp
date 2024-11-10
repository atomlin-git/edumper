#pragma once

typedef struct _CURDIR
{
    UNICODE_STRING DosPath;                                         //0x0
    VOID* Handle;                                                           //0x10
}CURDIR, * PCURDIR;

typedef struct _RTL_DRIVE_LETTER_CURDIR
{
    USHORT Flags;                                                           //0x0
    USHORT Length;                                                          //0x2
    ULONG TimeStamp;                                                        //0x4
    STRING DosPath;                                                 //0x8
}RTL_DRIVE_LETTER_CURDIR, * PRTL_DRIVE_LETTER_CURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
    ULONG MaximumLength;                                                    //0x0
    ULONG Length;                                                           //0x4
    ULONG Flags;                                                            //0x8
    ULONG DebugFlags;                                                       //0xc
    VOID* ConsoleHandle;                                                    //0x10
    ULONG ConsoleFlags;                                                     //0x18
    VOID* StandardInput;                                                    //0x20
    VOID* StandardOutput;                                                   //0x28
    VOID* StandardError;                                                    //0x30
    CURDIR CurrentDirectory;                                        //0x38
    UNICODE_STRING DllPath;                                         //0x50
    UNICODE_STRING ImagePathName;                                   //0x60
    UNICODE_STRING CommandLine;                                     //0x70
    VOID* Environment;                                                      //0x80
    ULONG StartingX;                                                        //0x88
    ULONG StartingY;                                                        //0x8c
    ULONG CountX;                                                           //0x90
    ULONG CountY;                                                           //0x94
    ULONG CountCharsX;                                                      //0x98
    ULONG CountCharsY;                                                      //0x9c
    ULONG FillAttribute;                                                    //0xa0
    ULONG WindowFlags;                                                      //0xa4
    ULONG ShowWindowFlags;                                                  //0xa8
    UNICODE_STRING WindowTitle;                                     //0xb0
    UNICODE_STRING DesktopInfo;                                     //0xc0
    UNICODE_STRING ShellInfo;                                       //0xd0
    UNICODE_STRING RuntimeData;                                     //0xe0
    RTL_DRIVE_LETTER_CURDIR CurrentDirectores[32];                  //0xf0
    ULONGLONG EnvironmentSize;                                              //0x3f0
    ULONGLONG EnvironmentVersion;                                           //0x3f8
    VOID* PackageDependencyData;                                            //0x400
    ULONG ProcessGroupId;                                                   //0x408
    ULONG LoaderThreads;                                                    //0x40c
    UNICODE_STRING RedirectionDllName;                              //0x410
    UNICODE_STRING HeapPartitionName;                               //0x420
    ULONGLONG* DefaultThreadpoolCpuSetMasks;                                //0x430
    ULONG DefaultThreadpoolCpuSetMaskCount;                                 //0x438
}RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

typedef struct _RTL_CRITICAL_SECTION_DEBUG
{
    USHORT Type;                                                            //0x0
    USHORT CreatorBackTraceIndex;                                           //0x2
    VOID* CriticalSection;                                  //0x8
    LIST_ENTRY ProcessLocksList;                                            //0x10
    ULONG EntryCount;                                                       //0x20
    ULONG ContentionCount;                                                  //0x24
    ULONG Flags;                                                            //0x28
    USHORT CreatorBackTraceIndexHigh;                                       //0x2c
    USHORT SpareUSHORT;                                                     //0x2e
}RTL_CRITICAL_SECTION_DEBUG, * PRTL_CRITICAL_SECTION_DEBUG;


typedef struct _RTL_CRITICAL_SECTION
{
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;                          //0x0
    LONG LockCount;                                                         //0x8
    LONG RecursionCount;                                                    //0xc
    VOID* OwningThread;                                                     //0x10
    VOID* LockSemaphore;                                                    //0x18
    ULONGLONG SpinCount;                                                    //0x20
}RTL_CRITICAL_SECTION, * PRTL_CRITICAL_SECTION;