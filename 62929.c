does_person_exist(const person_t* person)
{
	int i;

	for (i = 0; i < s_num_persons; ++i)
		if (person == s_persons[i]) return true;
	return false;
}
