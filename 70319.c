uint32_t GPMF_ElementsInStruct(GPMF_stream *ms)
{
	if (ms && ms->pos+1 < ms->buffer_size_longs)
	{
		uint32_t ssize = GPMF_StructSize(ms);
		GPMF_SampleType type = GPMF_SAMPLE_TYPE(ms->buffer[ms->pos + 1]);

		if (type != GPMF_TYPE_NEST && type != GPMF_TYPE_COMPLEX)
		{
			int32_t tsize = GPMF_SizeofType(type);
			if (tsize > 0)
				return ssize / tsize;
			else
				return 0;
		}

		if (type == GPMF_TYPE_COMPLEX)
		{
			GPMF_stream find_stream;
			GPMF_CopyState(ms, &find_stream);

			if (GPMF_OK == GPMF_FindPrev(&find_stream, GPMF_KEY_TYPE, GPMF_CURRENT_LEVEL))
			{
				char tmp[64] = "";
				uint32_t tmpsize = sizeof(tmp);
				char *data = (char *)GPMF_RawData(&find_stream);
				int size = GPMF_RawDataSize(&find_stream);
				
				if (GPMF_OK == GPMF_ExpandComplexTYPE(data, size, tmp, &tmpsize))
					return tmpsize;
			}
		}
	}
	return 0;
}
