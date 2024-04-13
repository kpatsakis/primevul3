struct ns_common *ns_get_owner(struct ns_common *ns)
{
	struct user_namespace *my_user_ns = current_user_ns();
	struct user_namespace *owner, *p;

	/* See if the owner is in the current user namespace */
	owner = p = ns->ops->owner(ns);
	for (;;) {
		if (!p)
			return ERR_PTR(-EPERM);
		if (p == my_user_ns)
			break;
		p = p->parent;
	}

	return &get_user_ns(owner)->ns;
}
