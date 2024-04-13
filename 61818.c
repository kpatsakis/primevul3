void jas_iccprof_destroy(jas_iccprof_t *prof)
{
	if (prof->attrtab)
		jas_iccattrtab_destroy(prof->attrtab);
	if (prof->tagtab.ents)
		jas_free(prof->tagtab.ents);
	jas_free(prof);
}
