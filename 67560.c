void __init update_intr_gate(unsigned int n, const void *addr)
{
	if (WARN_ON_ONCE(!test_bit(n, system_vectors)))
		return;
	set_intr_gate(n, addr);
}
