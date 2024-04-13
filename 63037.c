person_set_leader(person_t* person, person_t* leader, int distance)
{
	const person_t* node;

	if (leader != NULL) {
		node = leader;
		do {
			if (node == person)
				return false;
		} while ((node = node->leader));
	}

	if (leader != NULL) {
		if (!enlarge_step_history(leader, distance))
			return false;
		person->leader = leader;
		person->follow_distance = distance;
	}
	person->leader = leader;
	return true;
}
