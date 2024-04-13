int X86_immediate_size(unsigned int id)
{
#if 0
	unsigned int i;

	for (i = 0; i < ARR_SIZE(x86_imm_size); i++) {
		if (id == x86_imm_size[i].id) {
			return x86_imm_size[i].size;
		}
	}
#endif

	unsigned int left, right, m;

	left = 0;
	right = ARR_SIZE(x86_imm_size) - 1;

	while(left <= right) {
		m = (left + right) / 2;
		if (id == x86_imm_size[m].id)
			return x86_imm_size[m].size;

		if (id < x86_imm_size[m].id)
			right = m - 1;
		else
			left = m + 1;
	}

	return 0;
}
