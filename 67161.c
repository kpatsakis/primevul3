static int avr_custom_spm_page_fill(RAnalEsil *esil) {
	CPU_MODEL *cpu;
	ut64 addr, page_size_bits, i;
	ut8 r0, r1;

	if (!esil || !esil->anal || !esil->anal->reg) {
		return false;
	}

	if (!__esil_pop_argument(esil, &addr)) {
		return false;
	}

	if (!__esil_pop_argument (esil, &i)) {
		return false;
	}
	r0 = i;

	if (!__esil_pop_argument (esil, &i)) {
		return false;
	}
	r1 = i;

	cpu = get_cpu_model (esil->anal->cpu);
	page_size_bits = const_get_value (const_by_name (cpu, CPU_CONST_PARAM, "page_size"));

	addr &= (MASK (page_size_bits) ^ 1);

	r_anal_esil_mem_write (esil, addr++, &r0, 1);
	r_anal_esil_mem_write (esil, addr++, &r1, 1);

	return true;
}
