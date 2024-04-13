static int userns_install(struct nsproxy *nsproxy, struct ns_common *ns)
{
	struct user_namespace *user_ns = to_user_ns(ns);
	struct cred *cred;

	/* Don't allow gaining capabilities by reentering
	 * the same user namespace.
	 */
	if (user_ns == current_user_ns())
		return -EINVAL;

	/* Tasks that share a thread group must share a user namespace */
	if (!thread_group_empty(current))
		return -EINVAL;

	if (current->fs->users != 1)
		return -EINVAL;

	if (!ns_capable(user_ns, CAP_SYS_ADMIN))
		return -EPERM;

	cred = prepare_creds();
	if (!cred)
		return -ENOMEM;

	put_user_ns(cred->user_ns);
	set_cred_user_ns(cred, get_user_ns(user_ns));

	return commit_creds(cred);
}
