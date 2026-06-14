;
;
;

macro RegisterClassEx_macro _wndproc, _style, _class {
	mov dword [wc+00], 48
	mov dword [wc+04], _style
	mov dword [wc+08], _wndproc
	mov dword [wc+12], 0
	mov dword [wc+16], 0
	inl push 0, call dword [GetModuleHandle]
	mov dword [wc+20], eax
	mov dword [wc+24], 0

	inl push IDC_ARROW, push 0
	call dword [LoadCursor]

	mov dword [wc+28], 0
	mov dword [wc+32], COLOR_BTNFACE
	mov dword [wc+36], 0
	mov dword [wc+40], _class
	mov dword [wc+44], 0

	push wc
	call dword [RegisterClassEx]
}

macro CreateWindowEx_macro _w, _h, _st, _title, _class, hwnd {
	push 0
	push dword [wc+20]
	push 0
	push 0
	push _h
	push _w
	push CW_USEDEFAULT
	push CW_USEDEFAULT
	push _st
	push _title
	push _class
	push 0
	call dword [CreateWindowEx]
	mov [hwnd], eax
}
