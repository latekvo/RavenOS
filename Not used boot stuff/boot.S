#MIGIC
.set MAGIC, 0x1BADB002

.set FLAGS, 0

.set CHECKSUM, -(MAGIC + FLAGS)

#fields done

#multiboot
.section .multiboot

#MAGIC, FLAG, etc type
.long MAGIC
.long FLAGS
.long CHECKSUM

#stack

stackBottom:
.skip 512

stackTop:
.section .text
.global _start
.type _start, @function

_start:
	
	mov $stackTop, esp

	call KERNEL_MAIN

	cli

#infinite loop
hltLoop:
	hlt
	jmp hltLoop

.size _start, . - _start
	

