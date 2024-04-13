static void free_user_ns(struct work_struct *work)
{
	struct user_namespace *parent, *ns =
		container_of(work, struct user_namespace, work);

	do {
		struct ucounts *ucounts = ns->ucounts;
		parent = ns->parent;
		if (ns->gid_map.nr_extents > UID_GID_MAP_MAX_BASE_EXTENTS) {
			kfree(ns->gid_map.forward);
			kfree(ns->gid_map.reverse);
		}
		if (ns->uid_map.nr_extents > UID_GID_MAP_MAX_BASE_EXTENTS) {
			kfree(ns->uid_map.forward);
			kfree(ns->uid_map.reverse);
		}
		if (ns->projid_map.nr_extents > UID_GID_MAP_MAX_BASE_EXTENTS) {
			kfree(ns->projid_map.forward);
			kfree(ns->projid_map.reverse);
		}
		retire_userns_sysctls(ns);
#ifdef CONFIG_PERSISTENT_KEYRINGS
		key_put(ns->persistent_keyring_register);
#endif
		ns_free_inum(&ns->ns);
		kmem_cache_free(user_ns_cachep, ns);
		dec_user_namespaces(ucounts);
		ns = parent;
	} while (atomic_dec_and_test(&parent->count));
}
