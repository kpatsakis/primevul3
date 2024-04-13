person_activate(const person_t* person, person_op_t op, const person_t* acting_person, bool use_default)
{
	const person_t* last_acting;
	const person_t* last_current;

	last_acting = s_acting_person;
	last_current = s_current_person;
	s_acting_person = acting_person;
	s_current_person = person;
	if (use_default)
		script_run(s_def_person_scripts[op], false);
	if (does_person_exist(person))
		script_run(person->scripts[op], false);
	s_acting_person = last_acting;
	s_current_person = last_current;
}
