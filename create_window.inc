

CS_HREDRAW 				            = 00000002h
CS_VREDRAW 				            = 00000001h

; аргументы
; 1) указатель на wndproc	 ;
; 2) указатель на имя класса ;
RegisterClassEx_function:

	push ebp
	mov ebp, esp

	mov dword [wc+00], 48
	mov dword [wc+04], CS_HREDRAW+CS_VREDRAW

	mov eax, dword [ebp+12]
	mov dword [wc+08], eax
	mov dword [wc+12], 0
	mov dword [wc+16], 0

	push 0
	call dword [GetModuleHandle]
	mov dword [wc+20], eax

	mov dword [wc+24], 0
	mov dword [wc+28], 0

	mov dword [wc+32], COLOR_BTNFACE
	mov dword [wc+36], 0

	mov eax, dword [ebp+8]
	mov dword [wc+40], eax
	mov dword [wc+44], 0

	push wc
	call dword [RegisterClassEx]

	mov esp, ebp
	pop ebp
	ret 8

; Аргументы
; 1) widht
; 2) height
; 3) class
; 4) title
; push title
; push class
; push height
; push width
; call CreateWindowEx_function
CreateWindowEx_function:
	push 0
	push dword [wc+20]
	push 0
	push 0
	push HEIGHT
	push WIDTH
	push CW_USEDEFAULT
	push CW_USEDEFAULT
	push WS_OVERLAPPEDWINDOW+WS_VISIBLE
	push lpszWindowTitle
	push lpszClassName
	push WS_EX_COMPOSITED
	call dword [CreateWindowEx]
	ret
