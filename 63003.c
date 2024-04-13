person_following(const person_t* person, const person_t* leader)
{
	const person_t* node;

	node = person;
	while ((node = node->leader))
		if (node == leader) return true;
	return false;
}
