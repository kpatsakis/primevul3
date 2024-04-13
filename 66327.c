static void frag_dump_sample_flags(FILE * trace, u32 flags)
{
	fprintf(trace, " SamplePadding=\"%d\" Sync=\"%d\" DegradationPriority=\"%d\" IsLeading=\"%d\" DependsOn=\"%d\" IsDependedOn=\"%d\" HasRedundancy=\"%d\"",
	        GF_ISOM_GET_FRAG_PAD(flags), GF_ISOM_GET_FRAG_SYNC(flags), GF_ISOM_GET_FRAG_DEG(flags),
	        GF_ISOM_GET_FRAG_LEAD(flags), GF_ISOM_GET_FRAG_DEPENDS(flags), GF_ISOM_GET_FRAG_DEPENDED(flags), GF_ISOM_GET_FRAG_REDUNDANT(flags));
}
