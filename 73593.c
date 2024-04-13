void X86_reg_access(const cs_insn *insn,
		cs_regs regs_read, uint8_t *regs_read_count,
		cs_regs regs_write, uint8_t *regs_write_count)
{
	uint8_t i;
	uint8_t read_count, write_count;
	cs_x86 *x86 = &(insn->detail->x86);

	read_count = insn->detail->regs_read_count;
	write_count = insn->detail->regs_write_count;

	memcpy(regs_read, insn->detail->regs_read, read_count * sizeof(insn->detail->regs_read[0]));
	memcpy(regs_write, insn->detail->regs_write, write_count * sizeof(insn->detail->regs_write[0]));

	for (i = 0; i < x86->op_count; i++) {
		cs_x86_op *op = &(x86->operands[i]);
		switch((int)op->type) {
			case X86_OP_REG:
				if ((op->access & CS_AC_READ) && !arr_exist(regs_read, read_count, op->reg)) {
					regs_read[read_count] = op->reg;
					read_count++;
				}
				if ((op->access & CS_AC_WRITE) && !arr_exist(regs_write, write_count, op->reg)) {
					regs_write[write_count] = op->reg;
					write_count++;
				}
				break;
			case X86_OP_MEM:
				if ((op->mem.segment != X86_REG_INVALID)) {
					regs_read[read_count] = op->mem.segment;
					read_count++;
				}
				if ((op->mem.base != X86_REG_INVALID) && !arr_exist(regs_read, read_count, op->mem.base)) {
					regs_read[read_count] = op->mem.base;
					read_count++;
				}
				if ((op->mem.index != X86_REG_INVALID) && !arr_exist(regs_read, read_count, op->mem.index)) {
					regs_read[read_count] = op->mem.index;
					read_count++;
				}
			default:
				break;
		}
	}

	*regs_read_count = read_count;
	*regs_write_count = write_count;
}
