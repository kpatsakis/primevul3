GF_Err stsz_Size(GF_Box *s)
{
	u32 i, fieldSize, size;
	GF_SampleSizeBox *ptr = (GF_SampleSizeBox *)s;

	ptr->size += 8;
	if (!ptr->sampleCount) return GF_OK;

	if (ptr->type == GF_ISOM_BOX_TYPE_STSZ) {
		if (ptr->sampleSize) return GF_OK;
		ptr->size += (4 * ptr->sampleCount);
		return GF_OK;
	}

	fieldSize = 4;
	size = ptr->sizes[0];

	for (i=0; i < ptr->sampleCount; i++) {
		if (ptr->sizes[i] <= 0xF) continue;
		else if (ptr->sizes[i] <= 0xFF) {
			fieldSize = 8;
		}
		else if (ptr->sizes[i] <= 0xFFFF) {
			fieldSize = 16;
		}
		else {
			fieldSize = 32;
		}

		if (size != ptr->sizes[i]) size = 0;
	}
	if (size) {
		ptr->type = GF_ISOM_BOX_TYPE_STSZ;
		ptr->sampleSize = size;
		gf_free(ptr->sizes);
		ptr->sizes = NULL;
	}

	if (fieldSize == 32) {
		ptr->type = GF_ISOM_BOX_TYPE_STSZ;
		ptr->size += (4 * ptr->sampleCount);
		return GF_OK;
	}

	ptr->type = GF_ISOM_BOX_TYPE_STZ2;
	ptr->sampleSize = fieldSize;
	if (fieldSize == 4) {
		ptr->size += (ptr->sampleCount + 1) / 2;
	} else {
		ptr->size += (ptr->sampleCount) * (fieldSize/8);
	}
	return GF_OK;
}
