GPMF_ERR GPMF_Next(GPMF_stream *ms, GPMF_LEVELS recurse)
{
	if (ms)
	{
		if (ms->pos+1 < ms->buffer_size_longs)
		{

			uint32_t key, type = GPMF_SAMPLE_TYPE(ms->buffer[ms->pos + 1]);
			uint32_t size = (GPMF_DATA_SIZE(ms->buffer[ms->pos + 1]) >> 2);

			if (GPMF_OK != IsValidSize(ms, size)) return GPMF_ERROR_BAD_STRUCTURE;

			if (GPMF_TYPE_NEST == type && GPMF_KEY_DEVICE == ms->buffer[ms->pos] && ms->nest_level == 0)
			{
				ms->last_level_pos[ms->nest_level] = ms->pos;
				ms->nest_size[ms->nest_level] = size;
				if (recurse)
					ms->pos += 2;
				else
					ms->pos += 2 + size;
			}
			else
			{
				if (size + 2 > ms->nest_size[ms->nest_level])
					return GPMF_ERROR_BAD_STRUCTURE;

				if (recurse && type == GPMF_TYPE_NEST)
				{
					ms->last_level_pos[ms->nest_level] = ms->pos;
					ms->pos += 2;
					ms->nest_size[ms->nest_level] -= size + 2;

					ms->nest_level++;
					if (ms->nest_level > GPMF_NEST_LIMIT)
						return GPMF_ERROR_BAD_STRUCTURE;

					ms->nest_size[ms->nest_level] = size;
				}
				else
				{
					if (recurse)
					{
						ms->pos += size + 2;
						ms->nest_size[ms->nest_level] -= size + 2;
					}
					else
					{
						if (ms->nest_size[ms->nest_level] - (size + 2) > 0)
						{
							ms->pos += size + 2;
							ms->nest_size[ms->nest_level] -= size + 2;
						}
						else
						{
							return GPMF_ERROR_LAST;   
						}
					}
				}
			} 

			while (ms->pos < ms->buffer_size_longs && ms->nest_size[ms->nest_level] > 0 && ms->buffer[ms->pos] == GPMF_KEY_END)
			{
				ms->pos++;
				ms->nest_size[ms->nest_level]--;
			}

			while (ms->nest_level > 0 && ms->nest_size[ms->nest_level] == 0)
			{
				ms->nest_level--;
			}

			if (ms->pos < ms->buffer_size_longs)
			{
				while (ms->pos < ms->buffer_size_longs && ms->nest_size[ms->nest_level] > 0 && ms->buffer[ms->pos] == GPMF_KEY_END)
				{
					ms->pos++;
					ms->nest_size[ms->nest_level]--;
				}

				key = ms->buffer[ms->pos];
				if (!GPMF_VALID_FOURCC(key))
					return GPMF_ERROR_BAD_STRUCTURE;

				if (key == GPMF_KEY_DEVICE_ID)
					ms->device_id = BYTESWAP32(ms->buffer[ms->pos + 2]);
				if (key == GPMF_KEY_DEVICE_NAME)
				{
					size = GPMF_DATA_SIZE(ms->buffer[ms->pos + 1]); // in bytes
					if (size > sizeof(ms->device_name) - 1)
						size = sizeof(ms->device_name) - 1;
					memcpy(ms->device_name, &ms->buffer[ms->pos + 2], size);
					ms->device_name[size] = 0;
				}
			}
			else
			{
				return GPMF_ERROR_BUFFER_END;
			}

			return GPMF_OK;
		}
		else
		{
			return GPMF_ERROR_BUFFER_END;
		}
	}
	return GPMF_ERROR_MEMORY;
}
