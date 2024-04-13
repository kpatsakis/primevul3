static void cvardecs(JF, js_Ast *node)
{
	if (node->type == AST_LIST) {
		while (node) {
			cvardecs(J, F, node->a);
			node = node->b;
		}
		return;
	}

	if (isfun(node->type))
		return; /* stop at inner functions */

	if (node->type == EXP_VAR) {
		checkfutureword(J, F, node->a);
		addlocal(J, F, node->a, 1);
	}

	if (node->a) cvardecs(J, F, node->a);
	if (node->b) cvardecs(J, F, node->b);
	if (node->c) cvardecs(J, F, node->c);
	if (node->d) cvardecs(J, F, node->d);
}
