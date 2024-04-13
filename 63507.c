ssize_t proc_setgroups_write(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	struct seq_file *seq = file->private_data;
	struct user_namespace *ns = seq->private;
	char kbuf[8], *pos;
	bool setgroups_allowed;
	ssize_t ret;

	/* Only allow a very narrow range of strings to be written */
	ret = -EINVAL;
	if ((*ppos != 0) || (count >= sizeof(kbuf)))
		goto out;

	/* What was written? */
	ret = -EFAULT;
	if (copy_from_user(kbuf, buf, count))
		goto out;
	kbuf[count] = '\0';
	pos = kbuf;

	/* What is being requested? */
	ret = -EINVAL;
	if (strncmp(pos, "allow", 5) == 0) {
		pos += 5;
		setgroups_allowed = true;
	}
	else if (strncmp(pos, "deny", 4) == 0) {
		pos += 4;
		setgroups_allowed = false;
	}
	else
		goto out;

	/* Verify there is not trailing junk on the line */
	pos = skip_spaces(pos);
	if (*pos != '\0')
		goto out;

	ret = -EPERM;
	mutex_lock(&userns_state_mutex);
	if (setgroups_allowed) {
		/* Enabling setgroups after setgroups has been disabled
		 * is not allowed.
		 */
		if (!(ns->flags & USERNS_SETGROUPS_ALLOWED))
			goto out_unlock;
	} else {
		/* Permanently disabling setgroups after setgroups has
		 * been enabled by writing the gid_map is not allowed.
		 */
		if (ns->gid_map.nr_extents != 0)
			goto out_unlock;
		ns->flags &= ~USERNS_SETGROUPS_ALLOWED;
	}
	mutex_unlock(&userns_state_mutex);

	/* Report a successful write */
	*ppos = count;
	ret = count;
out:
	return ret;
out_unlock:
	mutex_unlock(&userns_state_mutex);
	goto out;
}
