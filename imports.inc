import_block:
    dd 0,0,0, RVA kernel_name, RVA kernel_iat
    dd 0,0,0, RVA user_name, RVA user_iat
    dd 0,0,0, RVA gdi_name, RVA gdi_iat
    dd 0,0,0, RVA gdiplus_name, RVA gdiplus_iat
    dd 0,0,0,0,0

    import_funcs kernel_iat, \
        ExitProcess, 'ExitProcess', \
        GetModuleHandle, 'GetModuleHandleA', \
        SetLastError, 'SetLastError', \
        GetLastError, 'GetLastError', \
        FormatMessage, 'FormatMessageA', \
        LocalFree, 'LocalFree'

    import_funcs user_iat, \
        MessageBoxA, 'MessageBoxA', \
        RegisterClassEx, 'RegisterClassExA', \
        CreateWindowEx, 'CreateWindowExA', \
        GetMessage, 'GetMessageA', \
        TranslateMessage, 'TranslateMessage', \
        DispatchMessage, 'DispatchMessageA', \
        PostQuitMessage, 'PostQuitMessage', \
        DefWindowProc, 'DefWindowProcA', \
        InvalidateRect, 'InvalidateRect', \
        BeginPaint, 'BeginPaint', \
        EndPaint, 'EndPaint', \
        LoadImage, 'LoadImageA', \
        LoadCursor, 'LoadCursorA', \
        ReleaseDC, 'ReleaseDC', \
        GetDC, 'GetDC'

    import_funcs gdi_iat, \
        TextOut, 'TextOutA', \
        CreateCompatibleDC, 'CreateCompatibleDC', \
        SelectObject, 'SelectObject', \
        DeleteDC, 'DeleteDC', \
        BitBlt, 'BitBlt', \
        CreateCompatibleBitmap, 'CreateCompatibleBitmap'

    import_funcs gdiplus_iat, \
        GdipCreateFromHDC, 'GdipCreateFromHDC', \
        GdipLoadImageFromFile, 'GdipLoadImageFromFile', \
        GdipDrawImage, 'GdipDrawImage', \
        GdipDeleteGraphics, 'GdipDeleteGraphics', \
        GdipDisposeImage, 'GdipDisposeImage', \
        GdiplusStartup, 'GdiplusStartup', \
        GdiplusShutdown, 'GdiplusShutdown'

    kernel_name db 'KERNEL32.DLL',0

    user_name db 'USER32.DLL',0

    gdi_name db 'GDI32.DLL',0

    gdiplus_name db 'GDIPLUS.DLL',0
