;------------------------------------------------------------------------------
; utils file
; AUTHOR: xinitrix
;------------------------------------------------------------------------------
macro inl [cmd] { cmd }

macro import_funcs dll_iat, [func_name, func_str_name] {

    common
    dll_iat:

    forward
        func_name:
        dd RVA func_name#_name

        common
        dd 0

        forward
            func_name#_name:
            dw 0
            db `func_str_name, 0
}

macro ShowLastError {
    call dword [GetLastError]
    mov dword [ErrorMsgID], eax

    inl push 0, push 0, push ErrorMsgPointer
    inl push 0, push [ErrorMsgID], push 0
    push FORMAT_MESSAGE_ALLOCATE_BUFFER+FORMAT_MESSAGE_FROM_SYSTEM
    call dword [FormatMessage]

    inl push 0, push 0, push [ErrorMsgPointer], push 0
    call dword [MessageBoxA]

    push dword [ErrorMsgPointer]
    call dword [LocalFree]
}
