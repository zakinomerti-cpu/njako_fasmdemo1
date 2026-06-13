format PE GUI 4.0
entry Start

include 'data.inc'

section '.code' code readable executable

include 'create_window.inc'

Start:

	push wndproc
	push lpszClassName
	call RegisterClassEx_function


	call CreateWindowEx_function
	mov [hWndMain], eax
	
	.mainloop:
		push 0
		push 0
		push 0
		push msg
		call dword [GetMessage]
		
		test eax, eax
		jz .exit
		
		push msg
		call dword [TranslateMessage]
		
		push msg
		call dword [DispatchMessage]
		jmp .mainloop
	
	.exit:
	push 0
	call dword [ExitProcess]

include 'wndproc.inc'

include 'utils.inc'

include 'imports.inc'
