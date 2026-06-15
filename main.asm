format PE GUI 4.0
entry Start
include 'utils.inc'
include 'create_window.inc'




section '.data' data readable writable
include 'data.inc'




section '.code' code readable executable

Start:
	RegisterClassEx_macro wndproc, \
		CS_HREDRAW+CS_VREDRAW, \
		lpszClassName
	CreateWindowEx_macro WIDTH, HEIGHT, \
		WS_VISIBLE+WS_OVERLAPPEDWINDOW, \
		lpszWindowTitle, lpszClassName, \
		hWndMain
	
	.mainloop:
		inl push 0, push 0, push 0, push msg
		call dword [GetMessage]
		
		test eax, eax
		jz .exit
		
		inl push msg, call dword [TranslateMessage]
		inl push msg, call dword [DispatchMessage]
		jmp .mainloop
	
	.exit:
		inl push 0, call dword [ExitProcess]

include 'wndproc_jpeg.inc'




section '.idata' import data readable writable
include 'imports.inc'
