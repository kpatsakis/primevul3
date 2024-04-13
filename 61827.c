static void jas_iccprof_sorttagtab(jas_icctagtab_t *tagtab)
{
	qsort(tagtab->ents, tagtab->numents, sizeof(jas_icctagtabent_t),
	  jas_icctagtabent_cmp);
}
