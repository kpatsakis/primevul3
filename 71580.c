static void cfunbody(JF, js_Ast *name, js_Ast *params, js_Ast *body)
{
	F->lightweight = 1;
	F->arguments = 0;

	if (F->script)
		F->lightweight = 0;

	/* Check if first statement is 'use strict': */
	if (body && body->type == AST_LIST && body->a && body->a->type == EXP_STRING)
		if (!strcmp(body->a->string, "use strict"))
			F->strict = 1;

	F->lastline = F->line;

	cparams(J, F, params, name);

	if (body) {
		cvardecs(J, F, body);
		cfundecs(J, F, body);
	}

	if (name) {
		checkfutureword(J, F, name);
		if (findlocal(J, F, name->string) < 0) {
			emit(J, F, OP_CURRENT);
			emit(J, F, OP_SETLOCAL);
			emitarg(J, F, addlocal(J, F, name, 0));
			emit(J, F, OP_POP);
		}
	}

	if (F->script) {
		emit(J, F, OP_UNDEF);
		cstmlist(J, F, body);
		emit(J, F, OP_RETURN);
	} else {
		cstmlist(J, F, body);
		emit(J, F, OP_UNDEF);
		emit(J, F, OP_RETURN);
	}
}
