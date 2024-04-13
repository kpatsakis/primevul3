map_engine_persons(void)
{
	int i;

	if (s_person_list == NULL)
		s_person_list = vector_new(sizeof(person_t*));
	vector_clear(s_person_list);
	for (i = 0; i < s_num_persons; ++i)
		vector_push(s_person_list, &s_persons[i]);
	return s_person_list;
}
