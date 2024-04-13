void ib_uverbs_detach_umcast(struct ib_qp *qp,
			     struct ib_uqp_object *uobj)
{
	struct ib_uverbs_mcast_entry *mcast, *tmp;

	list_for_each_entry_safe(mcast, tmp, &uobj->mcast_list, list) {
		ib_detach_mcast(qp, &mcast->gid, mcast->lid);
		list_del(&mcast->list);
		kfree(mcast);
	}
}
