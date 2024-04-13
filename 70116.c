int genl_unregister_family(const struct genl_family *family)
{
	genl_lock_all();

	if (!genl_family_find_byid(family->id)) {
		genl_unlock_all();
		return -ENOENT;
	}

	genl_unregister_mc_groups(family);

	idr_remove(&genl_fam_idr, family->id);

	up_write(&cb_lock);
	wait_event(genl_sk_destructing_waitq,
		   atomic_read(&genl_sk_destructing_cnt) == 0);
	genl_unlock();

	kfree(family->attrbuf);

	genl_ctrl_event(CTRL_CMD_DELFAMILY, family, NULL, 0);

	return 0;
}
