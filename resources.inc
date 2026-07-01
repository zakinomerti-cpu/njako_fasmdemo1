;lpsz
lpszClassName 			db 'window_class',0
lpszWindowTitle 		db 'winapi_window',0
hallo_msg				db 'Hallo',0

graphics_lib_path 		db 'offscreen.dll', 0
opengl_setup_name		db 'opengl_setup', 0
opengl_render_name		db 'opengl_render', 0
bg_path				    dw '1','.','p','n','g',0
njako_path			    dw '2','.','p','n','g',0
last_path			    dw '4','.','p','n','g',0
