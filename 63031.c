person_queue_script(person_t* person, script_t* script, bool is_immediate)
{
	++person->num_commands;
	if (person->num_commands > person->max_commands) {
		person->max_commands = person->num_commands * 2;
		if (!(person->commands = realloc(person->commands, person->max_commands * sizeof(struct command))))
			return false;
	}
	person->commands[person->num_commands - 1].type = COMMAND_RUN_SCRIPT;
	person->commands[person->num_commands - 1].is_immediate = is_immediate;
	person->commands[person->num_commands - 1].script = script;
	return true;
}
