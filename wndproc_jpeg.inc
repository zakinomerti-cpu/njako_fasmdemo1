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

    cmp eax, WM_KEYDOWN
    je .wm_keydown

    push dword [ebp+20] ; lparam lp
    push dword [ebp+16] ; wparam wp
    push dword [ebp+12] ; uint msg
    push dword [ebp+08] ; hwnd hwnd
    call dword [DefWindowProc]
    jmp .finish

    .wm_create:

        ; идеи по тому, как улучшить функцию:
        ; 1) создавать gdi+ obj в стеке
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

        push HEIGHT
        push WIDTH
        push dword [hdc+0]
        call dword [CreateCompatibleBitmap]
        mov dword [hBitMap+0], eax

        push dword [hBitMap+0]
        push dword [hdc+4]
        call dword [SelectObject]
        mov dword [hBitMap+4], eax

        push GpGraphics
        push dword [hdc+4]
        call dword [GdipCreateFromHDC]

        push GpImageBg
        push bg_path
        call dword [GdipLoadImageFromFile]

        push GpImageNjako
        push njako_path
        call dword [GdipLoadImageFromFile]

        push 768
        push 1024
        push 0
        push 0
        push [GpImageBg] ; скорее всего не будет работать
        push [GpGraphics] ; хз но оно заработало, думаю ошибка
        call dword [GdipDrawImageRectI]

        push 768
        push 1024
        push 0
        push 0
        push [GpImageNjako] ; скорее всего не будет работать
        push [GpGraphics] ; хз но оно заработало, думаю ошибка
        call dword [GdipDrawImageRectI]

        push [GpGraphics]
        call dword [GdipDeleteGraphics]

        push [GpImageBg]
        call dword [GdipDisposeImage]

        push [GpImageNjako]
        call dword [GdipDisposeImage]

        push dword [hdc+0]
        push dword [ebp+08]
        call dword [ReleaseDC]



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
        push HEIGHT
        push WIDTH
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

    .wm_keydown:
        cmp dword [ebp+16], 'Y'
        je .wm_keydown_y

        cmp dword [ebp+16], 'N'
        je .wm_keydown_n

        xor eax, eax
        jmp .finish

        .wm_keydown_n:
            jmp .wm_destroy
        .wm_keydown_y:

            ;;

            push dword [ebp+8]
            call dword [GetDC]
            mov dword [hdc+0], eax

            push dword [hBitMap+4]
            push dword [hdc+4]
            call dword [SelectObject]

            push dword [hBitMap+0]
            call dword [DeleteObject]

            push HEIGHT
            push WIDTH
            push dword [hdc+0]
            call dword [CreateCompatibleBitmap]
            mov dword [hBitMap+0], eax

            push dword [hBitMap+0]
            push dword [hdc+4]
            call dword [SelectObject]

            ;;

            push GpGraphics
            push dword [hdc+4]
            call dword [GdipCreateFromHDC]

            push GpImageBg
            push last_path
            call dword [GdipLoadImageFromFile]

            push 768
            push 1024
            push 0
            push 0
            push [GpImageBg] ; скорее всего не будет работать
            push [GpGraphics] ; хз но оно заработало, думаю ошибка
            call dword [GdipDrawImageRectI]

            push [GpGraphics]
            call dword [GdipDeleteGraphics]

            push [GpImageBg]
            call dword [GdipDisposeImage]

            ;;

            push dword [hdc+0]
            push dword [ebp+8]
            call dword [ReleaseDC]

            push 1
            push 0
            push dword [ebp+8]
            call dword [InvalidateRect]

            ;;
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

