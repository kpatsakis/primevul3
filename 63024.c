person_ignored_by(const person_t* person, const person_t* other)
{

	int i;

	if (other->ignore_all_persons || person->ignore_all_persons)
		return true;
	for (i = 0; i < other->num_ignores; ++i)
		if (strcmp(other->ignores[i], person->name) == 0) return true;
	for (i = 0; i < person->num_ignores; ++i)
		if (strcmp(person->ignores[i], other->name) == 0) return true;
	return false;
}
