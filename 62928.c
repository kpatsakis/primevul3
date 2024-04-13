detach_person(const person_t* person)
{
	int i;

	if (s_camera_person == person)
		s_camera_person = NULL;
	for (i = 0; i < PLAYER_MAX; ++i) {
		if (s_players[i].person == person)
			s_players[i].person = NULL;
	}
}
