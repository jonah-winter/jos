global inb
global outb

section .text

inb:
  mov dx, [esp+4]
  in al, dx
  ret

outb:
  mov al, [esp+4]
  mov dx, [esp+8]
  out dx, al
  ret
