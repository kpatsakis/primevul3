static void cunary(JF, js_Ast *exp, int opcode)
{
	cexp(J, F, exp->a);
	emitline(J, F, exp);
	emit(J, F, opcode);
}
