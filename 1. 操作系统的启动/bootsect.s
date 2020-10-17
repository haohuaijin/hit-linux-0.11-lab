entry start
start:	
    mov	ah,#0x03	! read cursor pos
	xor	bh,bh
	int	0x10

    mov cx,#26
    mov bl,#07
    mov bp,#msg
    mov ax,#0x7c0
    mov es,ax
    mov ax,#0x1301
    int 0x10 

load_setup:
	mov	dx,#0x0000		! drive 0, head 0
	mov	cx,#0x0002		! sector 2, track 0
	mov	bx,#0x0200		! address = 512, in INITSEG
	mov	ax,#0x0200+4    ! service 2, nr of sectors
	int	0x13			! read it

    jmpi 0,#0x07e0      ! jump to setup 0x07e0 = 0x07c0 + 0x0200
                        ! there is set cs to 0x07e0, in the setup the cs be used.

msg:
    .byte 13,10
    .ascii "HaoOS is loading ..."
    .byte 13,10,13,10

.org 510

boot_flag:
    .word 0xAA55












