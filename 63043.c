person_set_trailing(person_t* person, int distance)
{
	enlarge_step_history(person->leader, distance);
	person->follow_distance = distance;
}
