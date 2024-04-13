static int java_op(RAnal *anal, RAnalOp *op, ut64 addr, const ut8 *data, int len) {
	/* get opcode size */
	ut8 op_byte = data[0];
	int sz = JAVA_OPS[op_byte].size;
	if (!op) {
		return sz;
	}
	memset (op, '\0', sizeof (RAnalOp));
	IFDBG {
	}
	op->addr = addr;
	op->size = sz;
	op->id = data[0];
	op->type2 = JAVA_OPS[op_byte].op_type;
	op->type = r_anal_ex_map_anal_ex_to_anal_op_type (op->type2);
	if (op_byte == 0xaa || op_byte == 0xab) {
		java_switch_op (anal, op, addr, data, len);
	}
	/* TODO: 
	if (IN_SWITCH_OP) {
		NUM_CASES_SEEN++;
		if (NUM_CASES_SEEN == SWITCH_OP_CASES) IN_SWITCH_OP=0;
		op->addr = addr;
		op->size = 4;
		op->type2 = 0;
		op->type = R_ANAL_OP_TYPE_CASE
		op->eob = 0;
		return op->sizes;
	}
	*/

	op->eob = r_anal_ex_is_op_type_eop (op->type2);
	IFDBG {
		const char *ot_str = r_anal_optype_to_string (op->type);
		eprintf ("op_type2: %s @ 0x%04"PFMT64x" 0x%08"PFMT64x" op_type: (0x%02"PFMT64x") %s.\n",
			JAVA_OPS[op_byte].name, addr, (ut64)op->type2, (ut64)op->type,  ot_str);
	}

	if (len < 4) {
		return 0;
	}
	if (op->type == R_ANAL_OP_TYPE_CJMP) {
		op->jump = addr + (short)(USHORT (data, 1));
		op->fail = addr + sz;
		IFDBG eprintf ("%s jmpto 0x%04"PFMT64x"  failto 0x%04"PFMT64x".\n",
			JAVA_OPS[op_byte].name, op->jump, op->fail);
	} else if (op->type  == R_ANAL_OP_TYPE_JMP) {
		op->jump = addr + (short)(USHORT (data, 1));
		IFDBG eprintf ("%s jmpto 0x%04"PFMT64x".\n", JAVA_OPS[op_byte].name, op->jump);
	} else if ( (op->type & R_ANAL_OP_TYPE_CALL) == R_ANAL_OP_TYPE_CALL ) {
		op->jump = (int)(short)(USHORT (data, 1));
		op->fail = addr + sz;
	}

	return op->size;
}
