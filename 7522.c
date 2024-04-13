init_sg(int num_rqst, struct smb_rqst *rqst, u8 *sign)
{
	unsigned int sg_len;
	struct scatterlist *sg;
	unsigned int i;
	unsigned int j;
	unsigned int idx = 0;
	int skip;

	sg_len = 1;
	for (i = 0; i < num_rqst; i++)
		sg_len += rqst[i].rq_nvec + rqst[i].rq_npages;

	sg = kmalloc_array(sg_len, sizeof(struct scatterlist), GFP_KERNEL);
	if (!sg)
		return NULL;

	sg_init_table(sg, sg_len);
	for (i = 0; i < num_rqst; i++) {
		for (j = 0; j < rqst[i].rq_nvec; j++) {
			/*
			 * The first rqst has a transform header where the
			 * first 20 bytes are not part of the encrypted blob
			 */
			skip = (i == 0) && (j == 0) ? 20 : 0;
			smb2_sg_set_buf(&sg[idx++],
					rqst[i].rq_iov[j].iov_base + skip,
					rqst[i].rq_iov[j].iov_len - skip);
			}

		for (j = 0; j < rqst[i].rq_npages; j++) {
			unsigned int len, offset;

			rqst_page_get_length(&rqst[i], j, &len, &offset);
			sg_set_page(&sg[idx++], rqst[i].rq_pages[j], len, offset);
		}
	}
	smb2_sg_set_buf(&sg[idx], sign, SMB2_SIGNATURE_SIZE);
	return sg;
}