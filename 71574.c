static void cbinary(JF, js_Ast *exp, int opcode)
{
	cexp(J, F, exp->a);
	cexp(J, F, exp->b);
	emitline(J, F, exp);
	emit(J, F, opcode);
}
