static int parseOpcode(RAsm *a, const char *op, Opcode *out) {
	out->has_bnd = false;
	bool isrepop = false;
	if (!strncmp (op, "bnd ", 4)) {
		out->has_bnd = true;
		op += 4;
	}
	char *args = strchr (op, ' ');
	out->mnemonic = args ? r_str_ndup (op, args - op) : strdup (op);
	out->operands[0].type = out->operands[1].type = 0;
	out->operands[0].extended = out->operands[1].extended = false;
	out->operands[0].reg = out->operands[0].regs[0] = out->operands[0].regs[1] = X86R_UNDEFINED;
	out->operands[1].reg = out->operands[1].regs[0] = out->operands[1].regs[1] = X86R_UNDEFINED;
	out->operands[0].immediate = out->operands[1].immediate = 0;
	out->operands[0].sign = out->operands[1].sign = 1;
	out->operands[0].is_good_flag = out->operands[1].is_good_flag = true;
	out->is_short = false;
	out->operands_count = 0;
	if (args) {
		args++;
	} else {
		return 1;
	}
	if (!r_str_ncasecmp (args, "short", 5)) {
		out->is_short = true;
		args += 5;
	}
	if (!strncmp (out->mnemonic, "rep", 3)) {
		isrepop = true;
	}
	parseOperand (a, args, &(out->operands[0]), isrepop);
	out->operands_count = 1;
	while (out->operands_count < MAX_OPERANDS) {
		args = strchr (args, ',');
		if (!args) {
			break;
		}
		args++;
		parseOperand (a, args, &(out->operands[out->operands_count]), isrepop);
		out->operands_count++;
	}
	return 0;
}
