int ns_get_name(char *buf, size_t size, struct task_struct *task,
			const struct proc_ns_operations *ns_ops)
{
	struct ns_common *ns;
	int res = -ENOENT;
	ns = ns_ops->get(task);
	if (ns) {
		res = snprintf(buf, size, "%s:[%u]", ns_ops->name, ns->inum);
		ns_ops->put(ns);
	}
	return res;
}
