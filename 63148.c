static int assemble(RAsm *a, RAsmOp *ao, const char *str) {
	ut8 __data[32] = {0};
	ut8 *data = __data;
	char op[128];
	LookupTable *lt_ptr;
	int retval = -1;
	Opcode instr = {0};

	strncpy (op, str, sizeof (op) - 1);
	op[sizeof (op) - 1] = '\0';
	parseOpcode (a, op, &instr);
	for (lt_ptr = oplookup; strcmp (lt_ptr->mnemonic, "null"); lt_ptr++) {
		if (!r_str_casecmp (instr.mnemonic, lt_ptr->mnemonic)) {
			if (lt_ptr->opcode > 0) {
				if (!lt_ptr->only_x32 || a->bits != 64) {
					ut8 *ptr = (ut8 *)&lt_ptr->opcode;
					int i = 0;
					for (; i < lt_ptr->size; i++) {
						data[i] = ptr[lt_ptr->size - (i + 1)];
					}
					retval = lt_ptr->size;
				}
			} else {
				if (lt_ptr->opdo) {
					if (instr.has_bnd) {
						data[0] = 0xf2;
						data ++;
					}
					retval = lt_ptr->opdo (a, data, &instr);
					if (instr.has_bnd) {
						retval++;
					}
				}
			}
			break;
		}
	}
	r_asm_op_set_buf (ao, __data, retval);
	free (instr.mnemonic);
	return retval;
}
