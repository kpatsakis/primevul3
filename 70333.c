GPMF_ERR GPMF_ScaledData(GPMF_stream *ms, void *buffer, uint32_t buffersize, uint32_t sample_offset, uint32_t read_samples, GPMF_SampleType outputType)
{
	if (ms && buffer)
	{
		uint8_t *data = (uint8_t *)&ms->buffer[ms->pos + 2];
		uint8_t *output = (uint8_t *)buffer;
		uint32_t sample_size = GPMF_SAMPLE_SIZE(ms->buffer[ms->pos + 1]);
		uint32_t output_sample_size = GPMF_SizeofType(outputType);
		uint32_t remaining_sample_size = GPMF_DATA_PACKEDSIZE(ms->buffer[ms->pos + 1]);
		uint8_t type = GPMF_SAMPLE_TYPE(ms->buffer[ms->pos + 1]);
		char complextype[64] = "L";
		uint32_t inputtypesize = 0;
		uint32_t inputtypeelements = 0;
		uint8_t scaletype = 0;
		uint8_t scalecount = 0;
		uint32_t scaletypesize = 0;
		uint32_t *scaledata = NULL;
		uint32_t tmpbuffer[64];
		uint32_t tmpbuffersize = sizeof(tmpbuffer);
		uint32_t elements = 1;

		type = GPMF_SAMPLE_TYPE(ms->buffer[ms->pos + 1]);

		if (type == GPMF_TYPE_NEST)
			return GPMF_ERROR_MEMORY;

		if (GPMF_OK != IsValidSize(ms, remaining_sample_size >> 2))
			return GPMF_ERROR_BAD_STRUCTURE;

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
				uint32_t typestringlength = sizeof(complextype);
				if (GPMF_OK == GPMF_ExpandComplexTYPE(data1, size, complextype, &typestringlength))
				{
					inputtypeelements = elements = typestringlength;

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
			complextype[0] = type;
			inputtypesize = GPMF_SizeofType(type);
			if (inputtypesize == 0)
				return GPMF_ERROR_MEMORY;
			inputtypeelements = 1;
			elements = sample_size / inputtypesize;
		}

		if (output_sample_size * elements * read_samples > buffersize)
			return GPMF_ERROR_MEMORY;


		switch (outputType)	{
		case GPMF_TYPE_SIGNED_BYTE:
		case GPMF_TYPE_UNSIGNED_BYTE:
		case GPMF_TYPE_SIGNED_SHORT:
		case GPMF_TYPE_UNSIGNED_SHORT:
		case GPMF_TYPE_FLOAT:
		case GPMF_TYPE_SIGNED_LONG:
		case GPMF_TYPE_UNSIGNED_LONG:
		case GPMF_TYPE_DOUBLE:
		{
			GPMF_stream fs;
			GPMF_CopyState(ms, &fs);

			if (GPMF_OK == GPMF_FindPrev(&fs, GPMF_KEY_SCALE, GPMF_CURRENT_LEVEL))
			{
				scaledata = (uint32_t *)GPMF_RawData(&fs);
				scaletype = GPMF_SAMPLE_TYPE(fs.buffer[fs.pos + 1]);

				switch (scaletype)
				{
				case GPMF_TYPE_SIGNED_BYTE:
				case GPMF_TYPE_UNSIGNED_BYTE:
				case GPMF_TYPE_SIGNED_SHORT:
				case GPMF_TYPE_UNSIGNED_SHORT:
				case GPMF_TYPE_SIGNED_LONG:
				case GPMF_TYPE_UNSIGNED_LONG:
				case GPMF_TYPE_FLOAT:
					scalecount = GPMF_SAMPLES(fs.buffer[fs.pos + 1]);
					scaletypesize = GPMF_SizeofType(scaletype);

					if (scalecount > 1)
						if (scalecount != elements)
							return GPMF_ERROR_SCALE_COUNT;

					GPMF_FormattedData(&fs, tmpbuffer, tmpbuffersize, 0, scalecount);

					scaledata = (uint32_t *)tmpbuffer;
					break;
				default:
					return GPMF_ERROR_TYPE_NOT_SUPPORTED;
					break;
				}
			}
			else
			{
				scaletype = 'L';
				scalecount = 1;
				tmpbuffer[0] = 1; // set the scale to 1 is no scale was provided
				scaledata = (uint32_t *)tmpbuffer;
			}
		}

		while (read_samples--)
		{
			uint32_t i;
			uint8_t *scaledata8 = (uint8_t *)scaledata;

			for (i = 0; i < elements; i++)
			{
				switch (complextype[i % inputtypeelements])
				{
				case GPMF_TYPE_FLOAT:  MACRO_BSWAP_CAST_SCALE(BYTESWAP32, float, uint32_t) break;
				case GPMF_TYPE_SIGNED_BYTE:  MACRO_BSWAP_CAST_SCALE(NOSWAP8, int8_t, uint8_t) break;
				case GPMF_TYPE_UNSIGNED_BYTE:  MACRO_BSWAP_CAST_SCALE(NOSWAP8, uint8_t, uint8_t) break;
				case GPMF_TYPE_SIGNED_SHORT:  MACRO_BSWAP_CAST_SCALE(BYTESWAP16, int16_t, uint16_t) break;
				case GPMF_TYPE_UNSIGNED_SHORT:  MACRO_BSWAP_CAST_SCALE(BYTESWAP16, uint16_t, uint16_t) break;
				case GPMF_TYPE_SIGNED_LONG:  MACRO_BSWAP_CAST_SCALE(BYTESWAP32, int32_t, uint32_t) break;
				case GPMF_TYPE_UNSIGNED_LONG:  MACRO_BSWAP_CAST_SCALE(BYTESWAP32, uint32_t, uint32_t) break;
				case GPMF_TYPE_SIGNED_64BIT_INT:  MACRO_BSWAP_CAST_SCALE(BYTESWAP64, uint64_t, uint64_t) break;
				case GPMF_TYPE_UNSIGNED_64BIT_INT:  MACRO_BSWAP_CAST_SCALE(BYTESWAP64, uint64_t, uint64_t) break;
				default:
					return GPMF_ERROR_TYPE_NOT_SUPPORTED;
					break;
				}
				if (scalecount > 1)
					scaledata8 += scaletypesize;
			}
		}
		break;

		default:
			return GPMF_ERROR_TYPE_NOT_SUPPORTED;
			break;
		}

		return GPMF_OK;
	}

	return GPMF_ERROR_MEMORY;
}
