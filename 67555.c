void __init idt_setup_early_traps(void)
{
	idt_setup_from_table(idt_table, early_idts, ARRAY_SIZE(early_idts),
			     true);
	load_idt(&idt_descr);
}
