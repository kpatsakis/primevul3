static void checkfutureword(JF, js_Ast *exp)
{
	if (jsY_findword(exp->string, futurewords, nelem(futurewords)) >= 0)
		jsC_error(J, exp, "'%s' is a future reserved word", exp->string);
	if (F->strict) {
		if (jsY_findword(exp->string, strictfuturewords, nelem(strictfuturewords)) >= 0)
			jsC_error(J, exp, "'%s' is a strict mode future reserved word", exp->string);
	}
}
