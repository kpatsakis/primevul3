js_Function *jsC_compilescript(js_State *J, js_Ast *prog, int default_strict)
{
	return newfun(J, prog ? prog->line : 0, NULL, NULL, prog, 1, default_strict);
}
