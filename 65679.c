void clap_del(GF_Box *s)
{
	GF_CleanAppertureBox *ptr = (GF_CleanAppertureBox*)s;
	if (ptr == NULL) return;
	gf_free(ptr);
}
