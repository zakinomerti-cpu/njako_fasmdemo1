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

    push dword [ebp+20]
    push dword [ebp+16]
    push dword [ebp+12]
    push dword [ebp+08]
    call dword [DefWindowProc]
    jmp .finish

    .wm_create:
    push LR_LOADFROMFILE
    push 0
    push 0
    push IMAGE_BITMAP
    push image_path
    push 0
    call dword [LoadImage]
    mov dword [hBitMap+0], eax

    call ShowLastError
    jmp .finish

    .wm_paint:
    push ps
    push [hWndMain]
    call dword [BeginPaint]
    mov dword [hdc+0], eax

    ;TextOut(hdc, 0, 0, (LPSTR)"String", 6);
    push 5
    push hallo_msg
    push 0
    push 0
    push dword [hdc+0]
    call dword [TextOut]

    ;image blt
    ;if(hBitMap) {
        ;		HDC hdcMem = CreateCompatibleDC(hdc);
        ;		HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, g_hBitMap);
        ;		BitBlt(hdc, 0,0, 425, 600, hdcMem, 0, 0, SRCCOPY);
        ;
        ;		SelectObject(hdcMem, hOldBmp);
        ;		DeleteDC(hdcMem);
        ;	}

        push dword [hdc+0]
        call dword [CreateCompatibleDC]
        mov dword [hdc+4], eax

        push dword [hBitMap+0]
        push dword [hdc+4]
        call dword [SelectObject]
        mov dword [hBitMap+4], eax

        push SRCCOPY
        push 0
        push 0
        push dword [hdc+4]
        push 600
        push 425
        push 0
        push 0
        push dword [hdc]
        call dword [BitBlt]

        push dword [hBitMap+4]
        push dword [hdc+4]
        call dword [SelectObject]

        push dword [hdc+4]
        call dword [DeleteDC]

        push ps
        push [hWndMain]
        call dword [EndPaint]

        xor eax, eax
        jmp .finish

        .wm_destroy:
        push 0
        call dword [PostQuitMessage]
        xor eax, eax
        jmp .finish

        .finish:
        mov esp, ebp
        pop ebp
        ret 16

