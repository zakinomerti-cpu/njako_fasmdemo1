include 'resources.inc'

; variables
wc 						rb 48
msg 					rb 28
hWndMain 				dd 0

;
ErrorMsgPointer			dd 0
ErrorMsgID				dd 0

;wndproc
hBitMap					rb 8 ;2 hbitmap's
hdc						rb 8 ; 2 hdc's
ps						rb 64

BITMAPINFO  			db 44 dup (0)

pixelPtr				dd 0
hOffscreen				dd 0
opengl_setup			dd 0
opengl_render			dd 0

GpGraphics              dd 0
GpImageBg               dd 0
GpImageNjako            dd 0
gdiplusToken            dd 0
gdiplusStartupInput:
    dd 1
    dd 0
    dd 0
    dd 0

include 'consts.inc'
