#include "flt.h"
#include <string.h>

PFLT_FILTER flt_handle = NULL;

static BOOLEAN tgt(PUNICODE_STRING name) {
    if (!name || !name->Buffer) return FALSE;
    WCHAR buf[512];
    int n = name->Length / 2;
    if (n >= 512) n = 511;
    for (int i = 0; i < n; ++i) buf[i] = name->Buffer[i];
    buf[n] = 0;
    _wcslwr(buf);
    if (wcsstr(buf, L"\\cookies")) return TRUE;
    if (wcsstr(buf, L"login data")) return TRUE;
    if (wcsstr(buf, L"web data")) return TRUE;
    if (wcsstr(buf, L"local state")) return TRUE;
    if (wcsstr(buf, L"cookies.sqlite")) return TRUE;
    if (wcsstr(buf, L"logins.json")) return TRUE;
    if (wcsstr(buf, L"key4.db")) return TRUE;
    if (wcsstr(buf, L"leveldb")) return TRUE;
    if (wcsstr(buf, L"xk_cookie")) return TRUE;
    if (wcsstr(buf, L"dsc_tok")) return TRUE;
    return FALSE;
}

FLT_PREOP_CALLBACK flt_pre_create;
FLT_PREOP_CALLBACK flt_pre_read;
FLT_PREOP_CALLBACK flt_pre_write;
FLT_POSTOP_CALLBACK flt_post_cleanup;

CONST FLT_OPERATION_REGISTRATION ops[] = {
    { IRP_MJ_CREATE, 0, flt_pre_create, NULL },
    { IRP_MJ_READ, 0, flt_pre_read, NULL },
    { IRP_MJ_WRITE, 0, flt_pre_write, NULL },
    { IRP_MJ_CLEANUP, 0, NULL, flt_post_cleanup },
    { IRP_MJ_OPERATION_END }
};

CONST FLT_REGISTRATION reg = {
    sizeof(FLT_REGISTRATION),
    FLT_REGISTRATION_VERSION,
    0,
    NULL,
    ops,
    DriverEntry,
    flt_unload,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

NTSTATUS flt_unload(FLT_FILTER_UNLOAD_FLAGS flags) {
    UNREFERENCED_PARAMETER(flags);
    if (flt_handle) FltUnregisterFilter(flt_handle);
    return STATUS_SUCCESS;
}

FLT_PREOP_CALLBACK flt_pre_create {
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    PFLT_FILE_NAME_INFORMATION fi = NULL;
    if (FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &fi) == STATUS_SUCCESS) {
        if (FltParseFileNameInformation(fi) == STATUS_SUCCESS) {
            if (tgt(&fi->Name)) {
                FltReleaseFileNameInformation(fi);
                return FLT_PREOP_COMPLETE;
            }
        }
        FltReleaseFileNameInformation(fi);
    }
    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_PREOP_CALLBACK flt_pre_read {
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_PREOP_CALLBACK flt_pre_write {
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_POSTOP_CALLBACK flt_post_cleanup {
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    return FLT_POSTOP_FINISHED_processing;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT drv, PUNICODE_STRING reg) {
    UNREFERENCED_PARAMETER(reg);
    NTSTATUS st = FltRegisterFilter(drv, &reg, &flt_handle);
    if (!NT_SUCCESS(st)) return st;
    FltStartFiltering(flt_handle);
    return STATUS_SUCCESS;
}
