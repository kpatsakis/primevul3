int create_user_ns(struct cred *new)
{
	struct user_namespace *ns, *parent_ns = new->user_ns;
	kuid_t owner = new->euid;
	kgid_t group = new->egid;
	struct ucounts *ucounts;
	int ret, i;

	ret = -ENOSPC;
	if (parent_ns->level > 32)
		goto fail;

	ucounts = inc_user_namespaces(parent_ns, owner);
	if (!ucounts)
		goto fail;

	/*
	 * Verify that we can not violate the policy of which files
	 * may be accessed that is specified by the root directory,
	 * by verifing that the root directory is at the root of the
	 * mount namespace which allows all files to be accessed.
	 */
	ret = -EPERM;
	if (current_chrooted())
		goto fail_dec;

	/* The creator needs a mapping in the parent user namespace
	 * or else we won't be able to reasonably tell userspace who
	 * created a user_namespace.
	 */
	ret = -EPERM;
	if (!kuid_has_mapping(parent_ns, owner) ||
	    !kgid_has_mapping(parent_ns, group))
		goto fail_dec;

	ret = -ENOMEM;
	ns = kmem_cache_zalloc(user_ns_cachep, GFP_KERNEL);
	if (!ns)
		goto fail_dec;

	ret = ns_alloc_inum(&ns->ns);
	if (ret)
		goto fail_free;
	ns->ns.ops = &userns_operations;

	atomic_set(&ns->count, 1);
	/* Leave the new->user_ns reference with the new user namespace. */
	ns->parent = parent_ns;
	ns->level = parent_ns->level + 1;
	ns->owner = owner;
	ns->group = group;
	INIT_WORK(&ns->work, free_user_ns);
	for (i = 0; i < UCOUNT_COUNTS; i++) {
		ns->ucount_max[i] = INT_MAX;
	}
	ns->ucounts = ucounts;

	/* Inherit USERNS_SETGROUPS_ALLOWED from our parent */
	mutex_lock(&userns_state_mutex);
	ns->flags = parent_ns->flags;
	mutex_unlock(&userns_state_mutex);

#ifdef CONFIG_PERSISTENT_KEYRINGS
	init_rwsem(&ns->persistent_keyring_register_sem);
#endif
	ret = -ENOMEM;
	if (!setup_userns_sysctls(ns))
		goto fail_keyring;

	set_cred_user_ns(new, ns);
	return 0;
fail_keyring:
#ifdef CONFIG_PERSISTENT_KEYRINGS
	key_put(ns->persistent_keyring_register);
#endif
	ns_free_inum(&ns->ns);
fail_free:
	kmem_cache_free(user_ns_cachep, ns);
fail_dec:
	dec_user_namespaces(ucounts);
fail:
	return ret;
}
