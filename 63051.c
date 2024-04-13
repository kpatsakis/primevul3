sort_persons(void)
{
	qsort(s_persons, s_num_persons, sizeof(person_t*), compare_persons);
}
