smb3_free_compound_rqst(int num_rqst, struct smb_rqst *rqst)
{
	int i, j;

	for (i = 0; i < num_rqst; i++) {
		if (rqst[i].rq_pages) {
			for (j = rqst[i].rq_npages - 1; j >= 0; j--)
				put_page(rqst[i].rq_pages[j]);
			kfree(rqst[i].rq_pages);
		}
	}
}