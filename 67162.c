static int avr_custom_spm_page_write(RAnalEsil *esil) {
	CPU_MODEL *cpu;
	char *t = NULL;
	ut64 addr, page_size_bits, tmp_page;

	if (!esil || !esil->anal || !esil->anal->reg) {
		return false;
	}

	if (!__esil_pop_argument (esil, &addr)) {
		return false;
	}

	cpu = get_cpu_model (esil->anal->cpu);
	page_size_bits = const_get_value (const_by_name (cpu, CPU_CONST_PARAM, "page_size"));
	r_anal_esil_reg_read (esil, "_page", &tmp_page, NULL);

	addr &= (~(MASK (page_size_bits)) & CPU_PC_MASK (cpu));

	if (!(t = malloc (1 << page_size_bits))) {
		eprintf ("Cannot alloc a buffer for copying the temporary page.\n");
		return false;
	}
	r_anal_esil_mem_read (esil, tmp_page, (ut8 *) t, 1 << page_size_bits);
	r_anal_esil_mem_write (esil, addr, (ut8 *) t, 1 << page_size_bits);

	return true;
}
