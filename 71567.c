static int addlocal(JF, js_Ast *ident, int reuse)
{
	const char *name = ident->string;
	if (F->strict) {
		if (!strcmp(name, "arguments"))
			jsC_error(J, ident, "redefining 'arguments' is not allowed in strict mode");
		if (!strcmp(name, "eval"))
			jsC_error(J, ident, "redefining 'eval' is not allowed in strict mode");
	} else {
		if (!strcmp(name, "eval"))
			js_evalerror(J, "%s:%d: invalid use of 'eval'", J->filename, ident->line);
	}
	if (reuse || F->strict) {
		int i;
		for (i = 0; i < F->varlen; ++i) {
			if (!strcmp(F->vartab[i], name)) {
				if (reuse)
					return i+1;
				if (F->strict)
					jsC_error(J, ident, "duplicate formal parameter '%s'", name);
			}
		}
	}
	if (F->varlen >= F->varcap) {
		F->varcap = F->varcap ? F->varcap * 2 : 16;
		F->vartab = js_realloc(J, F->vartab, F->varcap * sizeof *F->vartab);
	}
	F->vartab[F->varlen] = name;
	return ++F->varlen;
}
