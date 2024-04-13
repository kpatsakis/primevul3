static void genl_unbind(struct net *net, int group)
{
	struct genl_family *f;
	unsigned int id;

	down_read(&cb_lock);

	idr_for_each_entry(&genl_fam_idr, f, id) {
		if (group >= f->mcgrp_offset &&
		    group < f->mcgrp_offset + f->n_mcgrps) {
			int fam_grp = group - f->mcgrp_offset;

			if (f->mcast_unbind)
				f->mcast_unbind(net, fam_grp);
			break;
		}
	}
	up_read(&cb_lock);
}
