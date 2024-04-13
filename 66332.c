GF_Err gf_isom_box_dump(void *ptr, FILE * trace)
{
	return gf_isom_box_dump_ex(ptr, trace, 0);
}
