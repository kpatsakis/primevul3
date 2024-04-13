person_ignore_list(person_t* person)
{

	int i;

	if (person->ignore_list == NULL)
		person->ignore_list = vector_new(sizeof(const char*));
	vector_clear(person->ignore_list);
	for (i = 0; i < person->num_ignores; ++i)
		vector_push(person->ignore_list, &person->ignores[i]);
	return person->ignore_list;
}
