person_queue_command(person_t* person, int command, bool is_immediate)
{
	struct command* commands;
	bool            is_aok = true;

	switch (command) {
	case COMMAND_MOVE_NORTHEAST:
		is_aok &= person_queue_command(person, COMMAND_MOVE_NORTH, true);
		is_aok &= person_queue_command(person, COMMAND_MOVE_EAST, is_immediate);
		return is_aok;
	case COMMAND_MOVE_SOUTHEAST:
		is_aok &= person_queue_command(person, COMMAND_MOVE_SOUTH, true);
		is_aok &= person_queue_command(person, COMMAND_MOVE_EAST, is_immediate);
		return is_aok;
	case COMMAND_MOVE_SOUTHWEST:
		is_aok &= person_queue_command(person, COMMAND_MOVE_SOUTH, true);
		is_aok &= person_queue_command(person, COMMAND_MOVE_WEST, is_immediate);
		return is_aok;
	case COMMAND_MOVE_NORTHWEST:
		is_aok &= person_queue_command(person, COMMAND_MOVE_NORTH, true);
		is_aok &= person_queue_command(person, COMMAND_MOVE_WEST, is_immediate);
		return is_aok;
	default:
		++person->num_commands;
		if (person->num_commands > person->max_commands) {
			if (!(commands = realloc(person->commands, person->num_commands * 2 * sizeof(struct command))))
				return false;
			person->max_commands = person->num_commands * 2;
			person->commands = commands;
		}
		person->commands[person->num_commands - 1].type = command;
		person->commands[person->num_commands - 1].is_immediate = is_immediate;
		person->commands[person->num_commands - 1].script = NULL;
		return true;
	}
}
