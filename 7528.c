init_read_bvec(struct page **pages, unsigned int npages, unsigned int data_size,
	       unsigned int cur_off, struct bio_vec **page_vec)
{
	struct bio_vec *bvec;
	int i;

	bvec = kcalloc(npages, sizeof(struct bio_vec), GFP_KERNEL);
	if (!bvec)
		return -ENOMEM;

	for (i = 0; i < npages; i++) {
		bvec[i].bv_page = pages[i];
		bvec[i].bv_offset = (i == 0) ? cur_off : 0;
		bvec[i].bv_len = min_t(unsigned int, PAGE_SIZE, data_size);
		data_size -= bvec[i].bv_len;
	}

	if (data_size != 0) {
		cifs_dbg(VFS, "%s: something went wrong\n", __func__);
		kfree(bvec);
		return -EIO;
	}

	*page_vec = bvec;
	return 0;
}