ShowLastError:
    call dword [GetLastError]
    mov dword [ErrorMsgID], eax

    push 0
    push 0
    push ErrorMsgPointer
    push 0
    push [ErrorMsgID]
    push 0
    push FORMAT_MESSAGE_ALLOCATE_BUFFER+FORMAT_MESSAGE_FROM_SYSTEM
    call dword [FormatMessage]

    test eax, eax
    jz .exit

    push 0
    push 0
    push [ErrorMsgPointer]
    push 0
    call dword [MessageBoxA]

    push dword [ErrorMsgPointer]
    call dword [LocalFree]

    .exit:
    ret
