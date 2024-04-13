c_next (struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;
	return c_start(m, pos);
}
