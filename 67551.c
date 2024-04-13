void __init idt_setup_apic_and_irq_gates(void)
{
	int i = FIRST_EXTERNAL_VECTOR;
	void *entry;

	idt_setup_from_table(idt_table, apic_idts, ARRAY_SIZE(apic_idts), true);

	for_each_clear_bit_from(i, system_vectors, FIRST_SYSTEM_VECTOR) {
		entry = irq_entries_start + 8 * (i - FIRST_EXTERNAL_VECTOR);
		set_intr_gate(i, entry);
	}

	for_each_clear_bit_from(i, system_vectors, NR_VECTORS) {
#ifdef CONFIG_X86_LOCAL_APIC
		set_bit(i, system_vectors);
		set_intr_gate(i, spurious_interrupt);
#else
		entry = irq_entries_start + 8 * (i - FIRST_EXTERNAL_VECTOR);
		set_intr_gate(i, entry);
#endif
	}
}
