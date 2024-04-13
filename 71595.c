static void emitlocal(JF, int oploc, int opvar, js_Ast *ident)
{
	int is_arguments = !strcmp(ident->string, "arguments");
	int is_eval = !strcmp(ident->string, "eval");
	int i;

	if (is_arguments) {
		F->lightweight = 0;
		F->arguments = 1;
	}

	checkfutureword(J, F, ident);
	if (F->strict && oploc == OP_SETLOCAL) {
		if (is_arguments)
			jsC_error(J, ident, "'arguments' is read-only in strict mode");
		if (is_eval)
			jsC_error(J, ident, "'eval' is read-only in strict mode");
	}
	if (is_eval)
		js_evalerror(J, "%s:%d: invalid use of 'eval'", J->filename, ident->line);

	i = findlocal(J, F, ident->string);
	if (i < 0) {
		emitstring(J, F, opvar, ident->string);
	} else {
		emit(J, F, oploc);
		emitarg(J, F, i);
	}
}
