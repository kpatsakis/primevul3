static void carray(JF, js_Ast *list)
{
	int i = 0;
	while (list) {
		if (list->a->type != EXP_UNDEF) {
			emitline(J, F, list->a);
			emitnumber(J, F, i++);
			cexp(J, F, list->a);
			emitline(J, F, list->a);
			emit(J, F, OP_INITPROP);
		} else {
			++i;
		}
		list = list->b;
	}
}
