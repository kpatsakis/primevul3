GF_Err gppc_dump(GF_Box *a, FILE * trace)
{
	GF_3GPPConfigBox *p = (GF_3GPPConfigBox *)a;
	const char *name = gf_4cc_to_str(p->cfg.vendor);
	switch (p->cfg.type) {
	case GF_ISOM_SUBTYPE_3GP_AMR:
	case GF_ISOM_SUBTYPE_3GP_AMR_WB:
		gf_isom_box_dump_start(a, "AMRConfigurationBox", trace);
		fprintf(trace, "Vendor=\"%s\" Version=\"%d\"", name, p->cfg.decoder_version);
		fprintf(trace, " FramesPerSample=\"%d\" SupportedModes=\"%x\" ModeRotating=\"%d\"", p->cfg.frames_per_sample, p->cfg.AMR_mode_set, p->cfg.AMR_mode_change_period);
		fprintf(trace, ">\n");
		gf_isom_box_dump_done("AMRConfigurationBox", a, trace);
		break;
	case GF_ISOM_SUBTYPE_3GP_EVRC:
		gf_isom_box_dump_start(a, "EVRCConfigurationBox", trace);
		fprintf(trace, "Vendor=\"%s\" Version=\"%d\" FramesPerSample=\"%d\" >\n", name, p->cfg.decoder_version, p->cfg.frames_per_sample);
		gf_isom_box_dump_done("EVRCConfigurationBox", a, trace);
		break;
	case GF_ISOM_SUBTYPE_3GP_QCELP:
		gf_isom_box_dump_start(a, "QCELPConfigurationBox", trace);
		fprintf(trace, "Vendor=\"%s\" Version=\"%d\" FramesPerSample=\"%d\" >\n", name, p->cfg.decoder_version, p->cfg.frames_per_sample);
		gf_isom_box_dump_done("QCELPConfigurationBox", a, trace);
		break;
	case GF_ISOM_SUBTYPE_3GP_SMV:
		gf_isom_box_dump_start(a, "SMVConfigurationBox", trace);
		fprintf(trace, "Vendor=\"%s\" Version=\"%d\" FramesPerSample=\"%d\" >\n", name, p->cfg.decoder_version, p->cfg.frames_per_sample);
		gf_isom_box_dump_done("SMVConfigurationBox", a, trace);
		break;
	case GF_ISOM_SUBTYPE_3GP_H263:
		gf_isom_box_dump_start(a, "H263ConfigurationBox", trace);
		fprintf(trace, "Vendor=\"%s\" Version=\"%d\"", name, p->cfg.decoder_version);
		fprintf(trace, " Profile=\"%d\" Level=\"%d\"", p->cfg.H263_profile, p->cfg.H263_level);
		fprintf(trace, ">\n");
		gf_isom_box_dump_done("H263ConfigurationBox", a, trace);
		break;
	default:
		break;
	}
	return GF_OK;
}
