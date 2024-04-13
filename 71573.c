static void cassignop2(JF, js_Ast *lhs, int postfix)
{
	switch (lhs->type) {
	case EXP_IDENTIFIER:
		emitline(J, F, lhs);
		if (postfix) emit(J, F, OP_ROT2);
		emitlocal(J, F, OP_SETLOCAL, OP_SETVAR, lhs);
		break;
	case EXP_INDEX:
		emitline(J, F, lhs);
		if (postfix) emit(J, F, OP_ROT4);
		emit(J, F, OP_SETPROP);
		break;
	case EXP_MEMBER:
		emitline(J, F, lhs);
		if (postfix) emit(J, F, OP_ROT3);
		emitstring(J, F, OP_SETPROP_S, lhs->b->string);
		break;
	default:
		jsC_error(J, lhs, "invalid l-value in assignment");
	}
}
