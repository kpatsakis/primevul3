GF_HEVCConfig *HEVC_DuplicateConfig(GF_HEVCConfig *cfg)
{
	char *data;
	u32 data_size;
	GF_HEVCConfig *new_cfg;
	GF_BitStream *bs;

	if (!cfg) return NULL;
	bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
	gf_odf_hevc_cfg_write_bs(cfg, bs);

	gf_bs_get_content(bs, &data, &data_size);
	gf_bs_del(bs);
	bs = gf_bs_new(data, data_size, GF_BITSTREAM_READ);

	new_cfg = gf_odf_hevc_cfg_read_bs(bs, cfg->is_lhvc);
	new_cfg->is_lhvc = cfg->is_lhvc;
	gf_bs_del(bs);
	gf_free(data);
	return new_cfg;
}
