bits 16
org 0x7C00

_start:
  cli
  ;mov ax, 0
  xor ax, ax
  mov ss, ax
  mov sp, 0x7C00

  mov [boot_drive], dl

  mov ax, 0x1000
  mov es, ax
  xor bx, bx

  mov ah, 0x02
  mov al, 0x0A
  mov ch, 0x00
  mov cl, 0x02
  mov dh, 0x00
  mov dl, [boot_drive]

  int 0x13
  jc disk_error

  in al, 0x92
  or al, 0b00000010
  out 0x92, al

  lgdt [gdt_descriptor]

  mov eax, cr0
  or eax, 1
  mov cr0, eax

  jmp 0x08:prot_mode

disk_error:
  mov byte [0xB800], 'E'
  mov byte [0xB801], 0x4F
  cli
  hlt
  jmp $

bits 32
prot_mode:
  mov ax, 0x10
  mov ds, ax

  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  mov esp, 0x90000

  call 0x10000

gdt_start:

gdt_null:
  dq 0

gdt_code:
  dw 0xFFFF
  dw 0
  db 0
  db 0b10011010
  db 0b11001111
  db 0

gdt_data:
  dw 0xFFFF
  dw 0
  db 0
  db 0b10010010
  db 0b11001111
  db 0

gdt_end:

gdt_descriptor:
  dw gdt_end-gdt_start-1
  dd gdt_start

boot_drive:
  db 0
times 510-($-$$) db 0
dw 0xAA55
