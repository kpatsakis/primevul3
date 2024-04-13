static int avr_custom_spm_page_erase(RAnalEsil *esil) {
	CPU_MODEL *cpu;
	ut8 c;
	ut64 addr, page_size_bits, i;

	if (!esil || !esil->anal || !esil->anal->reg) {
		return false;
	}

	if (!__esil_pop_argument(esil, &addr)) {
		return false;
	}

	cpu = get_cpu_model (esil->anal->cpu);
	page_size_bits = const_get_value (const_by_name (cpu, CPU_CONST_PARAM, "page_size"));

	addr &= ~(MASK (page_size_bits));

	c = 0xff;
	for (i = 0; i < (1ULL << page_size_bits); i++) {
		r_anal_esil_mem_write (
			esil, (addr + i) & CPU_PC_MASK (cpu), &c, 1);
	}

	return true;
}
