void sample_flags_dump(const char *name, u32 sample_flags, FILE * trace)
{
	fprintf(trace, "<%s", name);
	fprintf(trace, " IsLeading=\"%d\"", GF_ISOM_GET_FRAG_LEAD(sample_flags) );
	fprintf(trace, " SampleDependsOn=\"%d\"", GF_ISOM_GET_FRAG_DEPENDS(sample_flags) );
	fprintf(trace, " SampleIsDependedOn=\"%d\"", GF_ISOM_GET_FRAG_DEPENDED(sample_flags) );
	fprintf(trace, " SampleHasRedundancy=\"%d\"", GF_ISOM_GET_FRAG_REDUNDANT(sample_flags) );
	fprintf(trace, " SamplePadding=\"%d\"", GF_ISOM_GET_FRAG_PAD(sample_flags) );
	fprintf(trace, " SampleSync=\"%d\"", GF_ISOM_GET_FRAG_SYNC(sample_flags));
	fprintf(trace, " SampleDegradationPriority=\"%d\"", GF_ISOM_GET_FRAG_DEG(sample_flags));
	fprintf(trace, "/>\n");
}
