static void route4_walk(struct tcf_proto *tp, struct tcf_walker *arg,
			bool rtnl_held)
{
	struct route4_head *head = rtnl_dereference(tp->root);
	unsigned int h, h1;

	if (head == NULL || arg->stop)
		return;

	for (h = 0; h <= 256; h++) {
		struct route4_bucket *b = rtnl_dereference(head->table[h]);

		if (b) {
			for (h1 = 0; h1 <= 32; h1++) {
				struct route4_filter *f;

				for (f = rtnl_dereference(b->ht[h1]);
				     f;
				     f = rtnl_dereference(f->next)) {
					if (arg->count < arg->skip) {
						arg->count++;
						continue;
					}
					if (arg->fn(tp, f, arg) < 0) {
						arg->stop = 1;
						return;
					}
					arg->count++;
				}
			}
		}
	}
}