static int java_recursive_descent(RAnal *anal, RAnalState *state, ut64 addr) {
	RAnalBlock *bb;
	RAnalBlock *current_head;

	if (!anal || !state || !state->current_bb || state->current_bb_head)
		return 0;

	bb = state->current_bb;
	current_head = state->current_bb_head;

	if (current_head && state->current_bb->type & R_ANAL_BB_TYPE_TAIL) {
		r_anal_ex_update_bb_cfg_head_tail (current_head, current_head, state->current_bb);
	}

	if (bb->type2 & R_ANAL_EX_CODE_OP)  handle_bb_cf_recursive_descent (anal, state);
	return 0;
}
