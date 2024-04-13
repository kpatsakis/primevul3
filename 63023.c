person_ignore_name(person_t* person, const char* name)
{
	int index;

	index = person->num_ignores++;
	person->ignores = realloc(person->ignores, person->num_ignores * sizeof(char*));
	person->ignores[index] = strdup(name);

	vector_free(person->ignore_list);
	person->ignore_list = NULL;
}
