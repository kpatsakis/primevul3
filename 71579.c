static void cexp(JF, js_Ast *exp)
{
	int then, end;
	int n;

	switch (exp->type) {
	case EXP_STRING:
		emitline(J, F, exp);
		emitstring(J, F, OP_STRING, exp->string);
		break;
	case EXP_NUMBER:
		emitline(J, F, exp);
		emitnumber(J, F, exp->number);
		break;
	case EXP_UNDEF:
		emitline(J, F, exp);
		emit(J, F, OP_UNDEF);
		break;
	case EXP_NULL:
		emitline(J, F, exp);
		emit(J, F, OP_NULL);
		break;
	case EXP_TRUE:
		emitline(J, F, exp);
		emit(J, F, OP_TRUE);
		break;
	case EXP_FALSE:
		emitline(J, F, exp);
		emit(J, F, OP_FALSE);
		break;
	case EXP_THIS:
		emitline(J, F, exp);
		emit(J, F, OP_THIS);
		break;

	case EXP_REGEXP:
		emitline(J, F, exp);
		emit(J, F, OP_NEWREGEXP);
		emitarg(J, F, addstring(J, F, exp->string));
		emitarg(J, F, exp->number);
		break;

	case EXP_OBJECT:
		emitline(J, F, exp);
		emit(J, F, OP_NEWOBJECT);
		cobject(J, F, exp->a);
		break;

	case EXP_ARRAY:
		emitline(J, F, exp);
		emit(J, F, OP_NEWARRAY);
		carray(J, F, exp->a);
		break;

	case EXP_FUN:
		emitline(J, F, exp);
		emitfunction(J, F, newfun(J, exp->line, exp->a, exp->b, exp->c, 0, F->strict));
		break;

	case EXP_IDENTIFIER:
		emitline(J, F, exp);
		emitlocal(J, F, OP_GETLOCAL, OP_GETVAR, exp);
		break;

	case EXP_INDEX:
		cexp(J, F, exp->a);
		cexp(J, F, exp->b);
		emitline(J, F, exp);
		emit(J, F, OP_GETPROP);
		break;

	case EXP_MEMBER:
		cexp(J, F, exp->a);
		emitline(J, F, exp);
		emitstring(J, F, OP_GETPROP_S, exp->b->string);
		break;

	case EXP_CALL:
		ccall(J, F, exp->a, exp->b);
		break;

	case EXP_NEW:
		cexp(J, F, exp->a);
		n = cargs(J, F, exp->b);
		emitline(J, F, exp);
		emit(J, F, OP_NEW);
		emitarg(J, F, n);
		break;

	case EXP_DELETE:
		cdelete(J, F, exp);
		break;

	case EXP_PREINC:
		cassignop1(J, F, exp->a);
		emitline(J, F, exp);
		emit(J, F, OP_INC);
		cassignop2(J, F, exp->a, 0);
		break;

	case EXP_PREDEC:
		cassignop1(J, F, exp->a);
		emitline(J, F, exp);
		emit(J, F, OP_DEC);
		cassignop2(J, F, exp->a, 0);
		break;

	case EXP_POSTINC:
		cassignop1(J, F, exp->a);
		emitline(J, F, exp);
		emit(J, F, OP_POSTINC);
		cassignop2(J, F, exp->a, 1);
		emit(J, F, OP_POP);
		break;

	case EXP_POSTDEC:
		cassignop1(J, F, exp->a);
		emitline(J, F, exp);
		emit(J, F, OP_POSTDEC);
		cassignop2(J, F, exp->a, 1);
		emit(J, F, OP_POP);
		break;

	case EXP_VOID:
		cexp(J, F, exp->a);
		emitline(J, F, exp);
		emit(J, F, OP_POP);
		emit(J, F, OP_UNDEF);
		break;

	case EXP_TYPEOF: ctypeof(J, F, exp); break;
	case EXP_POS: cunary(J, F, exp, OP_POS); break;
	case EXP_NEG: cunary(J, F, exp, OP_NEG); break;
	case EXP_BITNOT: cunary(J, F, exp, OP_BITNOT); break;
	case EXP_LOGNOT: cunary(J, F, exp, OP_LOGNOT); break;

	case EXP_BITOR: cbinary(J, F, exp, OP_BITOR); break;
	case EXP_BITXOR: cbinary(J, F, exp, OP_BITXOR); break;
	case EXP_BITAND: cbinary(J, F, exp, OP_BITAND); break;
	case EXP_EQ: cbinary(J, F, exp, OP_EQ); break;
	case EXP_NE: cbinary(J, F, exp, OP_NE); break;
	case EXP_STRICTEQ: cbinary(J, F, exp, OP_STRICTEQ); break;
	case EXP_STRICTNE: cbinary(J, F, exp, OP_STRICTNE); break;
	case EXP_LT: cbinary(J, F, exp, OP_LT); break;
	case EXP_GT: cbinary(J, F, exp, OP_GT); break;
	case EXP_LE: cbinary(J, F, exp, OP_LE); break;
	case EXP_GE: cbinary(J, F, exp, OP_GE); break;
	case EXP_INSTANCEOF: cbinary(J, F, exp, OP_INSTANCEOF); break;
	case EXP_IN: cbinary(J, F, exp, OP_IN); break;
	case EXP_SHL: cbinary(J, F, exp, OP_SHL); break;
	case EXP_SHR: cbinary(J, F, exp, OP_SHR); break;
	case EXP_USHR: cbinary(J, F, exp, OP_USHR); break;
	case EXP_ADD: cbinary(J, F, exp, OP_ADD); break;
	case EXP_SUB: cbinary(J, F, exp, OP_SUB); break;
	case EXP_MUL: cbinary(J, F, exp, OP_MUL); break;
	case EXP_DIV: cbinary(J, F, exp, OP_DIV); break;
	case EXP_MOD: cbinary(J, F, exp, OP_MOD); break;

	case EXP_ASS: cassign(J, F, exp); break;
	case EXP_ASS_MUL: cassignop(J, F, exp, OP_MUL); break;
	case EXP_ASS_DIV: cassignop(J, F, exp, OP_DIV); break;
	case EXP_ASS_MOD: cassignop(J, F, exp, OP_MOD); break;
	case EXP_ASS_ADD: cassignop(J, F, exp, OP_ADD); break;
	case EXP_ASS_SUB: cassignop(J, F, exp, OP_SUB); break;
	case EXP_ASS_SHL: cassignop(J, F, exp, OP_SHL); break;
	case EXP_ASS_SHR: cassignop(J, F, exp, OP_SHR); break;
	case EXP_ASS_USHR: cassignop(J, F, exp, OP_USHR); break;
	case EXP_ASS_BITAND: cassignop(J, F, exp, OP_BITAND); break;
	case EXP_ASS_BITXOR: cassignop(J, F, exp, OP_BITXOR); break;
	case EXP_ASS_BITOR: cassignop(J, F, exp, OP_BITOR); break;

	case EXP_COMMA:
		cexp(J, F, exp->a);
		emitline(J, F, exp);
		emit(J, F, OP_POP);
		cexp(J, F, exp->b);
		break;

	case EXP_LOGOR:
		cexp(J, F, exp->a);
		emitline(J, F, exp);
		emit(J, F, OP_DUP);
		end = emitjump(J, F, OP_JTRUE);
		emit(J, F, OP_POP);
		cexp(J, F, exp->b);
		label(J, F, end);
		break;

	case EXP_LOGAND:
		cexp(J, F, exp->a);
		emitline(J, F, exp);
		emit(J, F, OP_DUP);
		end = emitjump(J, F, OP_JFALSE);
		emit(J, F, OP_POP);
		cexp(J, F, exp->b);
		label(J, F, end);
		break;

	case EXP_COND:
		cexp(J, F, exp->a);
		emitline(J, F, exp);
		then = emitjump(J, F, OP_JTRUE);
		cexp(J, F, exp->c);
		end = emitjump(J, F, OP_JUMP);
		label(J, F, then);
		cexp(J, F, exp->b);
		label(J, F, end);
		break;

	default:
		jsC_error(J, exp, "unknown expression: (%s)", jsP_aststring(exp->type));
	}
}
