static inline void idt_init_desc(gate_desc *gate, const struct idt_data *d)
{
	unsigned long addr = (unsigned long) d->addr;

	gate->offset_low	= (u16) addr;
	gate->segment		= (u16) d->segment;
	gate->bits		= d->bits;
	gate->offset_middle	= (u16) (addr >> 16);
#ifdef CONFIG_X86_64
	gate->offset_high	= (u32) (addr >> 32);
	gate->reserved		= 0;
#endif
}
