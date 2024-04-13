GPMF_ERR GPMF_Validate(GPMF_stream *ms, GPMF_LEVELS recurse)
{
	if (ms)
	{
		uint32_t currpos = ms->pos;
		int32_t nestsize = (int32_t)ms->nest_size[ms->nest_level];
		if (nestsize == 0 && ms->nest_level == 0)
			nestsize = ms->buffer_size_longs;
		
		while (ms->pos+1 < ms->buffer_size_longs && nestsize > 0)
		{
			uint32_t key = ms->buffer[ms->pos];

			if (ms->nest_level == 0 && key != GPMF_KEY_DEVICE && ms->device_count == 0 && ms->pos == 0)
			{
				DBG_MSG("ERROR: uninitized -- GPMF_ERROR_BAD_STRUCTURE\n");
				return GPMF_ERROR_BAD_STRUCTURE;
			}

			if (GPMF_VALID_FOURCC(key))
			{
				uint32_t type_size_repeat = ms->buffer[ms->pos + 1];
				int32_t size = GPMF_DATA_SIZE(type_size_repeat) >> 2;
				uint8_t type = GPMF_SAMPLE_TYPE(type_size_repeat);
				if (size + 2 > nestsize)
				{
					DBG_MSG("ERROR: nest size too small within %c%c%c%c-- GPMF_ERROR_BAD_STRUCTURE\n", PRINTF_4CC(key));
					return GPMF_ERROR_BAD_STRUCTURE;
				}

				if (!GPMF_VALID_FOURCC(key))
				{
					DBG_MSG("ERROR: invalid 4CC -- GPMF_ERROR_BAD_STRUCTURE\n");
					return GPMF_ERROR_BAD_STRUCTURE;
				}

				if (type == GPMF_TYPE_NEST && recurse == GPMF_RECURSE_LEVELS)
				{
					uint32_t validnest;
					ms->pos += 2;
					ms->nest_level++;
					if (ms->nest_level > GPMF_NEST_LIMIT)
					{
						DBG_MSG("ERROR: nest level within %c%c%c%c too deep -- GPMF_ERROR_BAD_STRUCTURE\n", PRINTF_4CC(key));
						return GPMF_ERROR_BAD_STRUCTURE;
					}
					ms->nest_size[ms->nest_level] = size;
					validnest = GPMF_Validate(ms, recurse);
					ms->nest_level--;
					if (GPMF_OK != validnest)
					{
						DBG_MSG("ERROR: invalid nest within %c%c%c%c -- GPMF_ERROR_BAD_STRUCTURE\n", PRINTF_4CC(key));
						return GPMF_ERROR_BAD_STRUCTURE;
					}
					else
					{
						if (ms->nest_level == 0)
							ms->device_count++;
					}

					ms->pos += size;
					nestsize -= 2 + size;

					while (ms->pos < ms->buffer_size_longs && nestsize > 0 && ms->buffer[ms->pos] == GPMF_KEY_END)
					{
						ms->pos++;
						nestsize--;
					}
				}
				else
				{
					ms->pos += 2 + size;
					nestsize -= 2 + size;
				}

				if (ms->pos == ms->buffer_size_longs)
				{
					ms->pos = currpos;
					return GPMF_OK;
				}
			}
			else
			{
				if (key == GPMF_KEY_END)
				{
					do
					{
						ms->pos++;
						nestsize--;
					} while (ms->pos < ms->buffer_size_longs && nestsize > 0 && ms->buffer[ms->pos] == 0);
				}
				else if (ms->nest_level == 0 && ms->device_count > 0)
				{
					ms->pos = currpos;
					return GPMF_OK;
				}
				else
				{
					DBG_MSG("ERROR: bad struct within %c%c%c%c -- GPMF_ERROR_BAD_STRUCTURE\n", PRINTF_4CC(key));
					return GPMF_ERROR_BAD_STRUCTURE;
				}
			}
		}

		ms->pos = currpos;
		return GPMF_OK;
	}
	else
	{
		DBG_MSG("ERROR: Invalid handle -- GPMF_ERROR_MEMORY\n");
		return GPMF_ERROR_MEMORY;
	}
}
