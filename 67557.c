void __init idt_setup_ist_traps(void)
{
	idt_setup_from_table(idt_table, ist_idts, ARRAY_SIZE(ist_idts), true);
}
