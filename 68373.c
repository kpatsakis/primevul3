void *ns_get_path(struct path *path, struct task_struct *task,
			const struct proc_ns_operations *ns_ops)
{
	struct ns_common *ns;
	void *ret;

again:
	ns = ns_ops->get(task);
	if (!ns)
		return ERR_PTR(-ENOENT);

	ret = __ns_get_path(path, ns);
	if (IS_ERR(ret) && PTR_ERR(ret) == -EAGAIN)
		goto again;
	return ret;
}
