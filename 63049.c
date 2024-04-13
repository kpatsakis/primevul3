reset_persons(bool keep_existing)
{
	unsigned int id;
	point3_t     origin;
	person_t*    person;

	int i, j;

	origin = map_origin();
	for (i = 0; i < s_num_persons; ++i) {
		person = s_persons[i];
		id = person->id;
		if (!keep_existing)
			person->num_commands = 0;
		if (person->is_persistent || keep_existing) {
			person->x = origin.x;
			person->y = origin.y;
			person->layer = origin.z;
		}
		else {
			person_activate(person, PERSON_SCRIPT_ON_DESTROY, NULL, true);
			free_person(person);
			--s_num_persons;
			for (j = i; j < s_num_persons; ++j)
				s_persons[j] = s_persons[j + 1];
			--i;
		}
	}
	sort_persons();
}
