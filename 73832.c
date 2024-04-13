int is_subdir(struct dentry *new_dentry, struct dentry *old_dentry)
{
	int result;
	unsigned seq;

	if (new_dentry == old_dentry)
		return 1;

	do {
		/* for restarting inner loop in case of seq retry */
		seq = read_seqbegin(&rename_lock);
		/*
		 * Need rcu_readlock to protect against the d_parent trashing
		 * due to d_move
		 */
		rcu_read_lock();
		if (d_ancestor(old_dentry, new_dentry))
			result = 1;
		else
			result = 0;
		rcu_read_unlock();
	} while (read_seqretry(&rename_lock, seq));

	return result;
}
