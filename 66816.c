void socket_seq_show(struct seq_file *seq)
{
	seq_printf(seq, "sockets: used %d\n",
		   sock_inuse_get(seq->private));
}
