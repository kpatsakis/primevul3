process_map_input(void)
{
	int       mv_x, mv_y;
	person_t* person;

	int i;

	kb_clear_queue();

	for (i = 0; i < PLAYER_MAX; ++i) {
		person = s_players[i].person;
		if (person != NULL) {
			if (kb_is_key_down(get_player_key(i, PLAYER_KEY_A))
				|| kb_is_key_down(s_players[i].talk_key)
				|| joy_is_button_down(i, s_talk_button))
			{
				if (s_players[i].is_talk_allowed)
					person_talk(person);
				s_players[i].is_talk_allowed = false;
			}
			else {
				s_players[i].is_talk_allowed = true;
			}
			mv_x = 0; mv_y = 0;
			if (person->num_commands == 0 && person->leader == NULL) {
				if (kb_is_key_down(get_player_key(i, PLAYER_KEY_UP)) || joy_position(i, 1) <= -0.5)
					mv_y = -1;
				if (kb_is_key_down(get_player_key(i, PLAYER_KEY_RIGHT)) || joy_position(i, 0) >= 0.5)
					mv_x = 1;
				if (kb_is_key_down(get_player_key(i, PLAYER_KEY_DOWN)) || joy_position(i, 1) >= 0.5)
					mv_y = 1;
				if (kb_is_key_down(get_player_key(i, PLAYER_KEY_LEFT)) || joy_position(i, 0) <= -0.5)
					mv_x = -1;
			}
			switch (mv_x + mv_y * 3) {
			case -3: // north
				person_queue_command(person, COMMAND_MOVE_NORTH, true);
				person_queue_command(person, COMMAND_FACE_NORTH, true);
				person_queue_command(person, COMMAND_ANIMATE, false);
				break;
			case -2: // northeast
				person_queue_command(person, COMMAND_MOVE_NORTHEAST, true);
				person_queue_command(person, COMMAND_FACE_NORTHEAST, true);
				person_queue_command(person, COMMAND_ANIMATE, false);
				break;
			case 1: // east
				person_queue_command(person, COMMAND_MOVE_EAST, true);
				person_queue_command(person, COMMAND_FACE_EAST, true);
				person_queue_command(person, COMMAND_ANIMATE, false);
				break;
			case 4: // southeast
				person_queue_command(person, COMMAND_MOVE_SOUTHEAST, true);
				person_queue_command(person, COMMAND_FACE_SOUTHEAST, true);
				person_queue_command(person, COMMAND_ANIMATE, false);
				break;
			case 3: // south
				person_queue_command(person, COMMAND_MOVE_SOUTH, true);
				person_queue_command(person, COMMAND_FACE_SOUTH, true);
				person_queue_command(person, COMMAND_ANIMATE, false);
				break;
			case 2: // southwest
				person_queue_command(person, COMMAND_MOVE_SOUTHWEST, true);
				person_queue_command(person, COMMAND_FACE_SOUTHWEST, true);
				person_queue_command(person, COMMAND_ANIMATE, false);
				break;
			case -1: // west
				person_queue_command(person, COMMAND_MOVE_WEST, true);
				person_queue_command(person, COMMAND_FACE_WEST, true);
				person_queue_command(person, COMMAND_ANIMATE, false);
				break;
			case -4: // northwest
				person_queue_command(person, COMMAND_MOVE_NORTHWEST, true);
				person_queue_command(person, COMMAND_FACE_NORTHWEST, true);
				person_queue_command(person, COMMAND_ANIMATE, false);
				break;
			}
		}
	}

	update_bound_keys(true);
}
