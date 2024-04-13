person_clear_ignores(person_t* person)
{
	int i;

	for (i = 0; i < person->num_ignores; ++i)
		free(person->ignores[i]);
	person->num_ignores = 0;
}
