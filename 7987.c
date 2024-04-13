static int route4_init(struct tcf_proto *tp)
{
	struct route4_head *head;

	head = kzalloc(sizeof(struct route4_head), GFP_KERNEL);
	if (head == NULL)
		return -ENOBUFS;

	rcu_assign_pointer(tp->root, head);
	return 0;
}