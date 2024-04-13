bool in_userns(const struct user_namespace *ancestor,
	       const struct user_namespace *child)
{
	const struct user_namespace *ns;
	for (ns = child; ns->level > ancestor->level; ns = ns->parent)
		;
	return (ns == ancestor);
}
