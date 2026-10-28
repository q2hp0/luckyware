#pragma once

#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>

#define POOL_TAG 'wlfa'

DRIVER_INITIALIZE DriverEntry;
NTSTATUS DriverEntry(PDRIVER_OBJECT drv, PUNICODE_STRING reg);
NTSTATUS flt_unload(FLT_FILTER_UNLOAD_FLAGS flags);
FLT_PREOP_CALLBACK flt_pre_create;
FLT_POSTOP_CALLBACK flt_post_create;
FLT_PREOP_CALLBACK flt_pre_read;
FLT_POSTOP_CALLBACK flt_pre_write;
FLT_POSTOP_CALLBACK flt_post_cleanup;
CONST FLT_OPERATION_REGISTRATION ops[];
CONST FLT_REGISTRATION reg;
PFLT_FILTER flt_handle;
ULONG ptrace_flag;
