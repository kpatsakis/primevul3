mdfour(unsigned char *md4_hash, unsigned char *link_str, int link_len)
{
	int rc;
	unsigned int size;
	struct crypto_shash *md4;
	struct sdesc *sdescmd4;

	md4 = crypto_alloc_shash("md4", 0, 0);
	if (IS_ERR(md4)) {
		rc = PTR_ERR(md4);
		cifs_dbg(VFS, "%s: Crypto md4 allocation error %d\n",
			 __func__, rc);
		return rc;
	}
	size = sizeof(struct shash_desc) + crypto_shash_descsize(md4);
	sdescmd4 = kmalloc(size, GFP_KERNEL);
	if (!sdescmd4) {
		rc = -ENOMEM;
		goto mdfour_err;
	}
	sdescmd4->shash.tfm = md4;
	sdescmd4->shash.flags = 0x0;

	rc = crypto_shash_init(&sdescmd4->shash);
	if (rc) {
		cifs_dbg(VFS, "%s: Could not init md4 shash\n", __func__);
		goto mdfour_err;
	}
	rc = crypto_shash_update(&sdescmd4->shash, link_str, link_len);
	if (rc) {
		cifs_dbg(VFS, "%s: Could not update with link_str\n", __func__);
		goto mdfour_err;
	}
	rc = crypto_shash_final(&sdescmd4->shash, md4_hash);
	if (rc)
		cifs_dbg(VFS, "%s: Could not generate md4 hash\n", __func__);

mdfour_err:
	crypto_free_shash(md4);
	kfree(sdescmd4);

	return rc;
}
