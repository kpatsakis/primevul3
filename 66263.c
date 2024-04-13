void video_sample_entry_del(GF_Box *s)
{
	GF_MPEGVisualSampleEntryBox *ptr = (GF_MPEGVisualSampleEntryBox *)s;
	if (ptr == NULL) return;
	gf_isom_sample_entry_predestroy((GF_SampleEntryBox *)s);

	if (ptr->esd) gf_isom_box_del((GF_Box *)ptr->esd);
	if (ptr->slc) gf_odf_desc_del((GF_Descriptor *)ptr->slc);
	/*for publishing*/
	if (ptr->emul_esd) gf_odf_desc_del((GF_Descriptor *)ptr->emul_esd);

	if (ptr->avc_config) gf_isom_box_del((GF_Box *) ptr->avc_config);
	if (ptr->svc_config) gf_isom_box_del((GF_Box *) ptr->svc_config);
	if (ptr->mvc_config) gf_isom_box_del((GF_Box *) ptr->mvc_config);
	if (ptr->hevc_config) gf_isom_box_del((GF_Box *) ptr->hevc_config);
	if (ptr->lhvc_config) gf_isom_box_del((GF_Box *) ptr->lhvc_config);
	if (ptr->cfg_3gpp) gf_isom_box_del((GF_Box *)ptr->cfg_3gpp);

	if (ptr->descr) gf_isom_box_del((GF_Box *) ptr->descr);
	if (ptr->ipod_ext) gf_isom_box_del((GF_Box *)ptr->ipod_ext);

	if (ptr->pasp) gf_isom_box_del((GF_Box *)ptr->pasp);
	if (ptr->clap) gf_isom_box_del((GF_Box *)ptr->clap);
	if (ptr->rinf) gf_isom_box_del((GF_Box *)ptr->rinf);

	if (ptr->rvcc) gf_isom_box_del((GF_Box *)ptr->rvcc);

	gf_free(ptr);
}
