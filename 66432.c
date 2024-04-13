GF_Err stbl_dump(GF_Box *a, FILE * trace)
{
	GF_SampleTableBox *p;
	p = (GF_SampleTableBox *)a;
	gf_isom_box_dump_start(a, "SampleTableBox", trace);
	fprintf(trace, ">\n");

	if (p->size)
		gf_isom_box_dump_ex(p->SampleDescription, trace, GF_ISOM_BOX_TYPE_STSD);
	if (p->size)
		gf_isom_box_dump_ex(p->TimeToSample, trace, GF_ISOM_BOX_TYPE_STTS);

	if (p->CompositionOffset) gf_isom_box_dump(p->CompositionOffset, trace);
	if (p->CompositionToDecode) gf_isom_box_dump(p->CompositionToDecode, trace);
	if (p->SyncSample) gf_isom_box_dump(p->SyncSample, trace);
	if (p->ShadowSync) gf_isom_box_dump(p->ShadowSync, trace);

	if (p->size)
		gf_isom_box_dump_ex(p->SampleToChunk, trace, GF_ISOM_BOX_TYPE_STSC);
	if (p->size)
		gf_isom_box_dump_ex(p->SampleSize, trace, GF_ISOM_BOX_TYPE_STSZ);
	if (p->size)
		gf_isom_box_dump_ex(p->ChunkOffset, trace, GF_ISOM_BOX_TYPE_STCO);

	if (p->DegradationPriority) gf_isom_box_dump(p->DegradationPriority, trace);
	if (p->SampleDep) gf_isom_box_dump(p->SampleDep, trace);
	if (p->PaddingBits) gf_isom_box_dump(p->PaddingBits, trace);
	if (p->Fragments) gf_isom_box_dump(p->Fragments, trace);
	if (p->sub_samples) gf_isom_box_array_dump(p->sub_samples, trace);
	if (p->sampleGroupsDescription) gf_isom_box_array_dump(p->sampleGroupsDescription, trace);
	if (p->sampleGroups) gf_isom_box_array_dump(p->sampleGroups, trace);
	if (p->sai_sizes) {
		u32 i;
		for (i = 0; i < gf_list_count(p->sai_sizes); i++) {
			GF_SampleAuxiliaryInfoSizeBox *saiz = (GF_SampleAuxiliaryInfoSizeBox *)gf_list_get(p->sai_sizes, i);
			gf_isom_box_dump(saiz, trace);
		}
	}

	if (p->sai_offsets) {
		u32 i;
		for (i = 0; i < gf_list_count(p->sai_offsets); i++) {
			GF_SampleAuxiliaryInfoOffsetBox *saio = (GF_SampleAuxiliaryInfoOffsetBox *)gf_list_get(p->sai_offsets, i);
			gf_isom_box_dump(saio, trace);
		}
	}

	gf_isom_box_dump_done("SampleTableBox", a, trace);
	return GF_OK;
}
