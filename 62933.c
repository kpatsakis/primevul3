free_person(person_t* person)
{
	int i;

	free(person->steps);
	for (i = 0; i < PERSON_SCRIPT_MAX; ++i)
		script_unref(person->scripts[i]);
	spriteset_unref(person->sprite);
	free(person->commands);
	free(person->name);
	free(person->direction);
	free(person);
}
