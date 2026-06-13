format PE GUI 4.0
entry Start

section '.data' data readable writable
;lpsz
lpszClassName 			db 'window_class',0
lpszWindowTitle 		db 'winapi_window',0
hallo_msg				db 'Hallo',0
image_path				db 'background.bmp',0

; variables
wc 						rb 48
msg 					rb 28
hWndMain 				dd 0
hBitMap					rb 8 ;2 hbitmap's
;
ErrorMsgPointer			dd 0
ErrorMsgID				dd 0
ps						rb 64
hdc						rb 8 ; 2 hdc's

; const
CS_HREDRAW 				= 00000002h
CS_VREDRAW 				= 00000001h
WS_OVERLAPPEDWINDOW 	= 00cf0000h
CW_USEDEFAULT			= 80000000h
WS_VISIBLE				= 10000000h
WS_EX_COMPOSITED		= 02000000h

COLOR_BTNFACE 			= 15
IDC_ARROW  				= 32512

WIDTH 					= 425
HEIGHT 					= 600

WM_DESTROY           	= 0002h
WM_PAINT             	= 000Fh
WM_CREATE            	= 0001h

FORMAT_MESSAGE_ALLOCATE_BUFFER 	= 00000100h
FORMAT_MESSAGE_FROM_SYSTEM		= 00001000h

LR_LOADFROMFILE     	= 0010h
IMAGE_BITMAP       		= 0
SRCCOPY     			= 00CC0020h

section '.code' code readable executable
Start:
	
	mov dword [wc+00], 48
	mov dword [wc+04], CS_HREDRAW+CS_VREDRAW
	mov dword [wc+08], wndproc
	mov dword [wc+12], 0
	mov dword [wc+16], 0
	
	push 0
	call dword [GetModuleHandle]
	mov dword [wc+20], eax
	
	mov dword [wc+24], 0
	mov dword [wc+28], 0
	
	mov dword [wc+32], COLOR_BTNFACE
	mov dword [wc+36], 0
	mov dword [wc+40], lpszClassName
	mov dword [wc+44], 0
	
	push wc
	call dword [RegisterClassEx]
	
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
	
	

section '.idata' import data readable writable
dd 0,0,0, RVA kernel_name, RVA kernel_iat
dd 0,0,0, RVA user_name, RVA user_iat
dd 0,0,0, RVA gdi_name, RVA gdi_iat
dd 0,0,0,0,0

kernel_iat:
	ExitProcess:
		dd RVA ExitProcess_name
	GetModuleHandle:
		dd RVA GetModuleHandle_name
	SetLastError:
		dd RVA SetLastError_name
	GetLastError:
		dd RVA GetLastError_name
	FormatMessage:
		dd RVA FormatMessage_name
	LocalFree:
		dd RVA LocalFree_name
	dd 0
	
GetModuleHandle_name:
	dw 0
	db 'GetModuleHandleA',0
ExitProcess_name:
	dw 0
	db 'ExitProcess',0
SetLastError_name:
	dw 0
	db 'SetLastError',0
GetLastError_name:
	dw 0
	db 'GetLastError',0
FormatMessage_name:
	dw 0
	db 'FormatMessageA',0
LocalFree_name:
	dw 0
	db 'LocalFree',0
	
	
	
	
user_iat:
	MessageBoxA:
		dd RVA MessageBoxA_name
	LoadCursor:
		dd RVA LoadCursor_name
	RegisterClassEx:
		dd RVA RegisterClassEx_name
	CreateWindowEx:
		dd RVA CreateWindowEx_name
	GetMessage:
		dd RVA GetMessage_name
	TranslateMessage:
		dd RVA TranslateMessage_name
	DispatchMessage:
		dd RVA DispatchMessage_name
	PostQuitMessage:
		dd RVA PostQuitMessage_name
	DefWindowProc:
		dd RVA DefWindowProc_name
	BeginPaint:
		dd RVA BeginPaint_name
	EndPaint:
		dd RVA EndPaint_name
	LoadImage:
		dd RVA LoadImage_name
	dd 0
	
MessageBoxA_name:
	dw 0
	db 'MessageBoxA',0
LoadCursor_name:
	dw 0
	db 'LoadCursorA',0
RegisterClassEx_name:
	dw 0
	db 'RegisterClassExA',0
CreateWindowEx_name:
	dw 0
	db 'CreateWindowExA',0
GetMessage_name:
	dw 0
	db 'GetMessageA',0
TranslateMessage_name:
	dw 0
	db 'TranslateMessage',0
DispatchMessage_name:
	dw 0
	db 'DispatchMessageA',0
PostQuitMessage_name:
	dw 0
	db 'PostQuitMessage',0
DefWindowProc_name:
	dw 0
	db 'DefWindowProcA',0
BeginPaint_name:
	dw 0
	db 'BeginPaint',0
EndPaint_name:
	dw 0
	db 'EndPaint',0
LoadImage_name:
	dw 0
	db 'LoadImageA',0
	
	
	
	
gdi_iat:
	TextOut:
		dd RVA TextOut_name
	CreateCompatibleDC:
		dd RVA CreateCompatibleDC_name
	SelectObject:
		dd RVA SelectObject_name
	BitBlt:
		dd RVA BitBlt_name
	DeleteDC:
		dd RVA DeleteDC_name
	
	dd 0
TextOut_name:
	dw 0
	db 'TextOutA',0
CreateCompatibleDC_name:
	dw 0
	db 'CreateCompatibleDC',0
SelectObject_name:
	dw 0
	db 'SelectObject',0
BitBlt_name:
	dw 0
	db 'BitBlt',0
DeleteDC_name:
	dw 0
	db 'DeleteDC',0

	
	
kernel_name:
	db 'KERNEL32.DLL',0
	
user_name:
	db 'USER32.DLL',0
	
gdi_name:
	db 'GDI32.DLL',0