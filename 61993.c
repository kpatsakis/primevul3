static void mif_cmpt_destroy(mif_cmpt_t *cmpt)
{
	if (cmpt->data) {
		jas_free(cmpt->data);
	}
	jas_free(cmpt);
}
