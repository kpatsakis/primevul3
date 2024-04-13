map_engine_set_player(player_id_t player_id, person_t* person)
{
	int i;

	for (i = 0; i < PLAYER_MAX; ++i) {
		if (s_players[i].person == person)
			s_players[i].person = NULL;
	}

	s_players[player_id].person = person;
}
