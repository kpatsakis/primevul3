GF_Err audio_sample_entry_Read(GF_Box *s, GF_BitStream *bs)
{
	GF_MPEGAudioSampleEntryBox *ptr;
	char *data;
	u32 i, size;
	GF_Err e;
	u64 pos;

	e = gf_isom_audio_sample_entry_read((GF_AudioSampleEntryBox*)s, bs);
	if (e) return e;
	pos = gf_bs_get_position(bs);
	size = (u32) s->size;

	e = gf_isom_box_array_read(s, bs, audio_sample_entry_AddBox);
	if (!e) return GF_OK;
	if (size<8) return GF_ISOM_INVALID_FILE;

	/*hack for some weird files (possibly recorded with live.com tools, needs further investigations)*/
	ptr = (GF_MPEGAudioSampleEntryBox *)s;
	gf_bs_seek(bs, pos);
	data = (char*)gf_malloc(sizeof(char) * size);
	gf_bs_read_data(bs, data, size);
	for (i=0; i<size-8; i++) {
		if (GF_4CC(data[i+4], data[i+5], data[i+6], data[i+7]) == GF_ISOM_BOX_TYPE_ESDS) {
			GF_BitStream *mybs = gf_bs_new(data + i, size - i, GF_BITSTREAM_READ);
			e = gf_isom_box_parse((GF_Box **)&ptr->esd, mybs);
			gf_bs_del(mybs);
			break;
		}
	}
	gf_free(data);
	return e;
}
