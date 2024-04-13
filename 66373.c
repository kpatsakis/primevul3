static void linf_dump(GF_LHVCLayerInformation *ptr, FILE * trace)
{
	u32 i, count;
	if (!ptr) {
		fprintf(trace, "<LayerInformation num_layers=\"\">\n");
		fprintf(trace, "<LayerInfoItem layer_id=\"\" min_temporalId=\"\" max_temporalId=\"\" sub_layer_presence_flags=\"\"/>\n");
		fprintf(trace, "</LayerInformation>\n");
		return;
	}

	count = gf_list_count(ptr->num_layers_in_track);
	fprintf(trace, "<LayerInformation num_layers=\"%d\">\n", count );
	for (i = 0; i < count; i++) {
		LHVCLayerInfoItem *li = (LHVCLayerInfoItem *)gf_list_get(ptr->num_layers_in_track, i);
		fprintf(trace, "<LayerInfoItem layer_id=\"%d\" min_temporalId=\"%d\" max_temporalId=\"%d\" sub_layer_presence_flags=\"%d\"/>\n", li->layer_id, li->min_TemporalId, li->max_TemporalId, li->sub_layer_presence_flags);
	}
	fprintf(trace, "</LayerInformation>\n");
	return;
}
