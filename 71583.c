static void cparams(JF, js_Ast *list, js_Ast *fname)
{
	F->numparams = listlength(list);
	while (list) {
		checkfutureword(J, F, list->a);
		addlocal(J, F, list->a, 0);
		list = list->b;
	}
}
