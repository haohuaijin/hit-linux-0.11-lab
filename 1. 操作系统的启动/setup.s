entry start
start:	
    mov	ah,#0x03	! read cursor pos
	xor	bh,bh
	int	0x10

    mov cx,#22 
    mov bl,#07
    mov bp,#msg1
    mov ax,cs       ! in there the cs = #0x07e0
    mov es,ax
    mov ax,#0x1301
    int 0x10 

    mov	ah,#0x03	! read cursor pos
	xor	bh,bh
	int	0x10

    mov bx,#0x0007
    mov cx,#2
    mov ah,#0x09
    mov al,#54   ! ascii 54 is the number 6
    int 0x10


msg1:
    .byte 13,10
    .ascii "Now is SETUP ..."
    .byte 13,10,13,10





