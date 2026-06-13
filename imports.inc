section '.idata' import data readable writable
    dd 0,0,0, RVA kernel_name, RVA kernel_iat
    dd 0,0,0, RVA user_name, RVA user_iat
    dd 0,0,0, RVA gdi_name, RVA gdi_iat
    dd 0,0,0,0,0

    kernel_iat:
    ExitProcess:
    dd RVA ExitProcess_name
    GetModuleHandle:
    dd RVA GetModuleHandle_name
    SetLastError:
    dd RVA SetLastError_name
    GetLastError:
    dd RVA GetLastError_name
    FormatMessage:
    dd RVA FormatMessage_name
    LocalFree:
    dd RVA LocalFree_name
    dd 0

    GetModuleHandle_name:
    dw 0
    db 'GetModuleHandleA',0
    ExitProcess_name:
    dw 0
    db 'ExitProcess',0
    SetLastError_name:
    dw 0
    db 'SetLastError',0
    GetLastError_name:
    dw 0
    db 'GetLastError',0
    FormatMessage_name:
    dw 0
    db 'FormatMessageA',0
    LocalFree_name:
    dw 0
    db 'LocalFree',0




    user_iat:
    MessageBoxA:
    dd RVA MessageBoxA_name
    LoadCursor:
    dd RVA LoadCursor_name
    RegisterClassEx:
    dd RVA RegisterClassEx_name
    CreateWindowEx:
    dd RVA CreateWindowEx_name
    GetMessage:
    dd RVA GetMessage_name
    TranslateMessage:
    dd RVA TranslateMessage_name
    DispatchMessage:
    dd RVA DispatchMessage_name
    PostQuitMessage:
    dd RVA PostQuitMessage_name
    DefWindowProc:
    dd RVA DefWindowProc_name
    BeginPaint:
    dd RVA BeginPaint_name
    EndPaint:
    dd RVA EndPaint_name
    LoadImage:
    dd RVA LoadImage_name
    dd 0

    MessageBoxA_name:
    dw 0
    db 'MessageBoxA',0
    LoadCursor_name:
    dw 0
    db 'LoadCursorA',0
    RegisterClassEx_name:
    dw 0
    db 'RegisterClassExA',0
    CreateWindowEx_name:
    dw 0
    db 'CreateWindowExA',0
    GetMessage_name:
    dw 0
    db 'GetMessageA',0
    TranslateMessage_name:
    dw 0
    db 'TranslateMessage',0
    DispatchMessage_name:
    dw 0
    db 'DispatchMessageA',0
    PostQuitMessage_name:
    dw 0
    db 'PostQuitMessage',0
    DefWindowProc_name:
    dw 0
    db 'DefWindowProcA',0
    BeginPaint_name:
    dw 0
    db 'BeginPaint',0
    EndPaint_name:
    dw 0
    db 'EndPaint',0
    LoadImage_name:
    dw 0
    db 'LoadImageA',0




    gdi_iat:
    TextOut:
    dd RVA TextOut_name
    CreateCompatibleDC:
    dd RVA CreateCompatibleDC_name
    SelectObject:
    dd RVA SelectObject_name
    BitBlt:
    dd RVA BitBlt_name
    DeleteDC:
    dd RVA DeleteDC_name

    dd 0
    TextOut_name:
    dw 0
    db 'TextOutA',0
    CreateCompatibleDC_name:
    dw 0
    db 'CreateCompatibleDC',0
    SelectObject_name:
    dw 0
    db 'SelectObject',0
    BitBlt_name:
    dw 0
    db 'BitBlt',0
    DeleteDC_name:
    dw 0
    db 'DeleteDC',0



    kernel_name:
    db 'KERNEL32.DLL',0

    user_name:
    db 'USER32.DLL',0

    gdi_name:
    db 'GDI32.DLL',0
