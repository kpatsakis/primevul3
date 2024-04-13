static long ns_ioctl(struct file *filp, unsigned int ioctl,
			unsigned long arg)
{
	struct user_namespace *user_ns;
	struct ns_common *ns = get_proc_ns(file_inode(filp));
	uid_t __user *argp;
	uid_t uid;

	switch (ioctl) {
	case NS_GET_USERNS:
		return open_related_ns(ns, ns_get_owner);
	case NS_GET_PARENT:
		if (!ns->ops->get_parent)
			return -EINVAL;
		return open_related_ns(ns, ns->ops->get_parent);
	case NS_GET_NSTYPE:
		return ns->ops->type;
	case NS_GET_OWNER_UID:
		if (ns->ops->type != CLONE_NEWUSER)
			return -EINVAL;
		user_ns = container_of(ns, struct user_namespace, ns);
		argp = (uid_t __user *) arg;
		uid = from_kuid_munged(current_user_ns(), user_ns->owner);
		return put_user(uid, argp);
	default:
		return -ENOTTY;
	}
}
