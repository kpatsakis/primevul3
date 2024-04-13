static void cvarinit(JF, js_Ast *list)
{
	while (list) {
		js_Ast *var = list->a;
		if (var->b) {
			cexp(J, F, var->b);
			emitline(J, F, var);
			emitlocal(J, F, OP_SETLOCAL, OP_SETVAR, var->a);
			emit(J, F, OP_POP);
		}
		list = list->b;
	}
}
