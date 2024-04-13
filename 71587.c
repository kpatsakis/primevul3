static void ctypeof(JF, js_Ast *exp)
{
	if (exp->a->type == EXP_IDENTIFIER) {
		emitline(J, F, exp->a);
		emitlocal(J, F, OP_GETLOCAL, OP_HASVAR, exp->a);
	} else {
		cexp(J, F, exp->a);
	}
	emitline(J, F, exp);
	emit(J, F, OP_TYPEOF);
}
