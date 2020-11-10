/*first entry - bootloader main*/
ENTRY(_start)

SECTIONS
{
	/*we need 1MB*/
	. = 1M
	/*text section*/
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
	/*read only*/
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
	/* DATA */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
	/*bss section*/
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(.COMMON)
		*(.bss)
	}
}
