static void set_intr_gate(unsigned int n, const void *addr)
{
	struct idt_data data;

	BUG_ON(n > 0xFF);

	memset(&data, 0, sizeof(data));
	data.vector	= n;
	data.addr	= addr;
	data.segment	= __KERNEL_CS;
	data.bits.type	= GATE_INTERRUPT;
	data.bits.p	= 1;

	idt_setup_from_table(idt_table, &data, 1, false);
}
