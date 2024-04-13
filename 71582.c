static void cobject(JF, js_Ast *list)
{
	js_Ast *head = list;

	while (list) {
		js_Ast *kv = list->a;
		js_Ast *prop = kv->a;

		if (prop->type == AST_IDENTIFIER || prop->type == EXP_STRING) {
			emitline(J, F, prop);
			emitstring(J, F, OP_STRING, prop->string);
		} else if (prop->type == EXP_NUMBER) {
			emitline(J, F, prop);
			emitnumber(J, F, prop->number);
		} else {
			jsC_error(J, prop, "invalid property name in object initializer");
		}

		if (F->strict)
			checkdup(J, F, head, kv);

		switch (kv->type) {
		default: /* impossible */ break;
		case EXP_PROP_VAL:
			cexp(J, F, kv->b);
			emitline(J, F, kv);
			emit(J, F, OP_INITPROP);
			break;
		case EXP_PROP_GET:
			emitfunction(J, F, newfun(J, prop->line, NULL, NULL, kv->c, 0, F->strict));
			emitline(J, F, kv);
			emit(J, F, OP_INITGETTER);
			break;
		case EXP_PROP_SET:
			emitfunction(J, F, newfun(J, prop->line, NULL, kv->b, kv->c, 0, F->strict));
			emitline(J, F, kv);
			emit(J, F, OP_INITSETTER);
			break;
		}

		list = list->b;
	}
}
