section '.data' data readable writable

include 'resources.inc'

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

include 'consts.inc'
