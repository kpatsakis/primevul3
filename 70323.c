GPMF_ERR GPMF_FormattedData(GPMF_stream *ms, void *buffer, uint32_t buffersize, uint32_t sample_offset, uint32_t read_samples)
{
	if (ms && buffer)
	{
		uint8_t *data = (uint8_t *)&ms->buffer[ms->pos + 2];
		uint8_t *output = (uint8_t *)buffer;
		uint32_t sample_size = GPMF_SAMPLE_SIZE(ms->buffer[ms->pos + 1]);
		uint32_t remaining_sample_size = GPMF_DATA_PACKEDSIZE(ms->buffer[ms->pos + 1]);
		uint8_t type = GPMF_SAMPLE_TYPE(ms->buffer[ms->pos + 1]);
		uint32_t typesize = 1;
		uint32_t elements = 0;
		uint32_t typestringlength = 1;
		char complextype[64] = "L";

		if (type == GPMF_TYPE_NEST)
			return GPMF_ERROR_BAD_STRUCTURE;
		
		if (GPMF_OK != IsValidSize(ms, remaining_sample_size>>2))
			return GPMF_ERROR_BAD_STRUCTURE;

		if (sample_size * read_samples > buffersize)
			return GPMF_ERROR_MEMORY;

		remaining_sample_size -= sample_offset * sample_size; // skip samples
		data += sample_offset * sample_size;

		if (remaining_sample_size < sample_size * read_samples)
			return GPMF_ERROR_MEMORY;

		if (type == GPMF_TYPE_COMPLEX)
		{
			GPMF_stream find_stream;
			GPMF_CopyState(ms, &find_stream);

			if (GPMF_OK == GPMF_FindPrev(&find_stream, GPMF_KEY_TYPE, GPMF_RECURSE_LEVELS))
			{
				char *data1 = (char *)GPMF_RawData(&find_stream);
				int size = GPMF_RawDataSize(&find_stream);

				typestringlength = sizeof(complextype);
				if (GPMF_OK == GPMF_ExpandComplexTYPE(data1, size, complextype, &typestringlength))
				{
					elements = (uint32_t)strlen(complextype);

					if (sample_size != GPMF_SizeOfComplexTYPE(complextype, typestringlength))
						return GPMF_ERROR_TYPE_NOT_SUPPORTED;
				}
				else
					return GPMF_ERROR_TYPE_NOT_SUPPORTED;
			}
			else
				return GPMF_ERROR_TYPE_NOT_SUPPORTED;
		}
		else
		{
			typesize = GPMF_SizeofType((GPMF_SampleType)type);

			if (type == GPMF_TYPE_FOURCC)
				typesize = 1; // Do not ByteSWAP

			if (typesize == 0)
				return GPMF_ERROR_MEMORY;

			elements = sample_size / typesize;
		}

		while (read_samples--)
		{
			uint32_t i,j;

			for (i = 0; i < elements; i++)
			{
				if (type == GPMF_TYPE_COMPLEX)
				{
					if (complextype[i] == GPMF_TYPE_FOURCC)
					{
						*output++ = *data++;
						*output++ = *data++;
						*output++ = *data++;
						*output++ = *data++;
						typesize = 0;
					}
					else
						typesize = GPMF_SizeofType(complextype[i]);
				}

				switch (typesize)
				{
				case 2:
				{
					uint16_t *data16 = (uint16_t *)data;
					uint16_t *output16 = (uint16_t *)output;
					*output16 = BYTESWAP16(*data16);
					output16++;
					data16++;

					data = (uint8_t *)data16;
					output = (uint8_t *)output16;
				}
				break;
				case 4:
				{
					uint32_t *data32 = (uint32_t *)data;
					uint32_t *output32 = (uint32_t *)output;
					*output32 = BYTESWAP32(*data32);
					output32++;
					data32++;

					data = (uint8_t *)data32;
					output = (uint8_t *)output32;
				}
				break;
				case 8:
				{
					uint32_t *data32 = (uint32_t *)data;
					uint32_t *output32 = (uint32_t *)output;
					*(output32+1) = BYTESWAP32(*data32);
					*(output32) = BYTESWAP32(*(data32+1));
					data32 += 2;
					output32 += 2;

					data = (uint8_t *)data32;
					output = (uint8_t *)output32;
				}
				break;
				default: //1, 16 or more not byteswapped
					for (j = 0; j < typesize; j++)
						*output++ = *data++;
					break;
				}
			}
		}

		return GPMF_OK;
	}

	return GPMF_ERROR_MEMORY;
}
