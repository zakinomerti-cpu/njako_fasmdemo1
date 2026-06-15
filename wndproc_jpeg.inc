;------------------------------------------------------------------------------
; main window proc
; AUTHOR: xinitrix
;------------------------------------------------------------------------------
wndproc:
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    cmp eax, WM_DESTROY
    je .wm_destroy

    cmp eax, WM_CREATE
    je .wm_create

    cmp eax, WM_PAINT
    je .wm_paint

    push dword [ebp+20] ; lparam lp
    push dword [ebp+16] ; wparam wp
    push dword [ebp+12] ; uint msg
    push dword [ebp+08] ; hwnd hwnd
    call dword [DefWindowProc]
    jmp .finish

    .wm_create:

        push 0
        push gdiplusStartupInput
        push gdiplusToken
        call dword [GdiplusStartup]

        push dword [ebp+08]
        call dword [GetDC]
        mov dword [hdc+0], eax

        push dword [hdc+0]
        call dword [CreateCompatibleDC]
        mov dword [hdc+4], eax

        push 600
        push 425
        push dword [hdc+0]
        call dword [CreateCompatibleBitmap]
        mov dword [hBitMap+0], eax

        push dword [hBitMap+0]
        push dword [hdc+4]
        call dword [SelectObject]

        push GpGraphics
        push dword [hdc+4]
        call dword [GdipCreateFromHDC]
        call .ShowLastError_proc

        push GpImage
        push image_path
        call dword [GdipLoadImageFromFile]
        call .ShowLastError_proc

        push 0
        push 0
        push [GpImage] ; скорее всего не будет работать
        push [GpGraphics] ; хз но оно заработало, думаю ошибка
        call dword [GdipDrawImage]
        call .ShowLastError_proc

        push dword [hdc+0]
        push dword [ebp+08]
        call dword [ReleaseDC]
        call .ShowLastError_proc



        xor eax, eax
        jmp .finish

    .wm_paint:

        push ps
        push dword [ebp+08]
        call dword [BeginPaint]
        mov dword [hdc+0], eax
        ;

        push SRCCOPY
        push 0
        push 0
        push dword [hdc+4]
        push 600
        push 425
        push 0
        push 0
        push dword [hdc+0]
        call dword [BitBlt]

        ;
        push ps
        push dword [ebp+08]
        call dword [EndPaint]

        xor eax, eax
        jmp .finish

    .wm_destroy:

        push [gdiplusToken]
        call dword [GdiplusShutdown]

        push 0
        call dword [PostQuitMessage]
        xor eax, eax
        jmp .finish

    .finish:
        mov esp, ebp
        pop ebp
        ret 16

    .showError:
        ShowLastError
        jmp .finish

    .ShowLastError_proc:
        ShowLastError
        ret






