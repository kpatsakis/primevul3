static int java_linear_sweep(RAnal *anal, RAnalState *state, ut64 addr) {
	RAnalBlock *bb = state->current_bb;
	if (state->current_bb_head && state->current_bb->type & R_ANAL_BB_TYPE_TAIL) {
	}

	if (bb->type2 & R_ANAL_EX_CODE_OP)  handle_bb_cf_linear_sweep (anal, state);
	return 0;
}
