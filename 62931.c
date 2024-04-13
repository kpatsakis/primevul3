enlarge_step_history(person_t* person, int new_size)
{
	struct step *new_steps;
	size_t      pastmost;
	double      last_x;
	double      last_y;

	int i;

	if (new_size > person->max_history) {
		if (!(new_steps = realloc(person->steps, new_size * sizeof(struct step))))
			return false;

		pastmost = person->max_history - 1;
		last_x = person->steps != NULL ? person->steps[pastmost].x : person->x;
		last_y = person->steps != NULL ? person->steps[pastmost].y : person->y;
		for (i = person->max_history; i < new_size; ++i) {
			new_steps[i].x = last_x;
			new_steps[i].y = last_y;
		}
		person->steps = new_steps;
		person->max_history = new_size;
	}

	return true;
}
