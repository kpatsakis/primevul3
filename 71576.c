static void cdelete(JF, js_Ast *exp)
{
	js_Ast *arg = exp->a;
	switch (arg->type) {
	case EXP_IDENTIFIER:
		if (F->strict)
			jsC_error(J, exp, "delete on an unqualified name is not allowed in strict mode");
		emitline(J, F, exp);
		emitlocal(J, F, OP_DELLOCAL, OP_DELVAR, arg);
		break;
	case EXP_INDEX:
		cexp(J, F, arg->a);
		cexp(J, F, arg->b);
		emitline(J, F, exp);
		emit(J, F, OP_DELPROP);
		break;
	case EXP_MEMBER:
		cexp(J, F, arg->a);
		emitline(J, F, exp);
		emitstring(J, F, OP_DELPROP_S, arg->b->string);
		break;
	default:
		jsC_error(J, exp, "invalid l-value in delete expression");
	}
}
