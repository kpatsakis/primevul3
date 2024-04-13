static int analyze_method(RAnal *anal, RAnalFunction *fcn, RAnalState *state) {
	r_list_free (fcn->bbs);
	fcn->bbs = r_anal_bb_list_new ();
	java_new_method (fcn->addr);
	state->current_fcn = fcn;
	r_anal_ex_perform_analysis (anal, state, fcn->addr);
	return state->anal_ret_val;
}
