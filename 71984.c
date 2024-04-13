GF_Err gen_sample_entry_Read(GF_Box *s, GF_BitStream *bs)
{
	return gf_isom_base_sample_entry_read((GF_SampleEntryBox *)s, bs);
}
