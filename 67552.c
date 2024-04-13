void __init idt_setup_debugidt_traps(void)
{
	memcpy(&debug_idt_table, &idt_table, IDT_ENTRIES * 16);

	idt_setup_from_table(debug_idt_table, dbg_idts, ARRAY_SIZE(dbg_idts), false);
}
