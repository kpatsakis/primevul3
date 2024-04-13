bool userns_may_setgroups(const struct user_namespace *ns)
{
	bool allowed;

	mutex_lock(&userns_state_mutex);
	/* It is not safe to use setgroups until a gid mapping in
	 * the user namespace has been established.
	 */
	allowed = ns->gid_map.nr_extents != 0;
	/* Is setgroups allowed? */
	allowed = allowed && (ns->flags & USERNS_SETGROUPS_ALLOWED);
	mutex_unlock(&userns_state_mutex);

	return allowed;
}
