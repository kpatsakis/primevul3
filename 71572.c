static void cassignop1(JF, js_Ast *lhs)
{
	switch (lhs->type) {
	case EXP_IDENTIFIER:
		emitline(J, F, lhs);
		emitlocal(J, F, OP_GETLOCAL, OP_GETVAR, lhs);
		break;
	case EXP_INDEX:
		cexp(J, F, lhs->a);
		cexp(J, F, lhs->b);
		emitline(J, F, lhs);
		emit(J, F, OP_DUP2);
		emit(J, F, OP_GETPROP);
		break;
	case EXP_MEMBER:
		cexp(J, F, lhs->a);
		emitline(J, F, lhs);
		emit(J, F, OP_DUP);
		emitstring(J, F, OP_GETPROP_S, lhs->b->string);
		break;
	default:
		jsC_error(J, lhs, "invalid l-value in assignment");
	}
}
