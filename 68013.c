GF_Box *gf_isom_clone_config_box(GF_Box *box)
{
	GF_Box *clone;
	switch (box->type)
	{
	case GF_ISOM_BOX_TYPE_AVCC:
	case GF_ISOM_BOX_TYPE_SVCC:
	case GF_ISOM_BOX_TYPE_MVCC:
		clone = gf_isom_box_new(box->type);
		((GF_AVCConfigurationBox *)clone)->config = AVC_DuplicateConfig(((GF_AVCConfigurationBox *)box)->config);
		break;
	case GF_ISOM_BOX_TYPE_HVCC:
		clone = gf_isom_box_new(box->type);
		((GF_HEVCConfigurationBox *)clone)->config = HEVC_DuplicateConfig(((GF_HEVCConfigurationBox *)box)->config);
		break;
	default:
		clone = NULL;
		GF_LOG(GF_LOG_WARNING, GF_LOG_CONTAINER, ("Cloning of config not supported for type %s\n", gf_4cc_to_str(box->type)));
		break;
	}
	return clone;
}
