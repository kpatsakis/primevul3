void alloc_intr_gate(unsigned int n, const void *addr)
{
	BUG_ON(n < FIRST_SYSTEM_VECTOR);
	if (!test_and_set_bit(n, system_vectors))
		set_intr_gate(n, addr);
}
