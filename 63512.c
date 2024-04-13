static struct ns_common *userns_get(struct task_struct *task)
{
	struct user_namespace *user_ns;

	rcu_read_lock();
	user_ns = get_user_ns(__task_cred(task)->user_ns);
	rcu_read_unlock();

	return user_ns ? &user_ns->ns : NULL;
}
