int proc_setgroups_show(struct seq_file *seq, void *v)
{
	struct user_namespace *ns = seq->private;
	unsigned long userns_flags = READ_ONCE(ns->flags);

	seq_printf(seq, "%s\n",
		   (userns_flags & USERNS_SETGROUPS_ALLOWED) ?
		   "allow" : "deny");
	return 0;
}
