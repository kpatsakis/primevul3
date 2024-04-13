person_free(person_t* person)
{
	int i, j;

	person_activate(person, PERSON_SCRIPT_ON_DESTROY, NULL, true);
	for (i = 0; i < s_num_persons; ++i) {
		if (s_persons[i]->leader == person)
			s_persons[i]->leader = NULL;
	}

	detach_person(person);
	for (i = 0; i < s_num_persons; ++i) {
		if (s_persons[i] == person) {
			for (j = i; j < s_num_persons - 1; ++j)
				s_persons[j] = s_persons[j + 1];
			--s_num_persons;
			--i;
		}
	}

	vector_free(person->ignore_list);

	free_person(person);
	sort_persons();
}
