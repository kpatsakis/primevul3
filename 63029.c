person_on_event(person_t* person, int type, script_t* script)
{
	script_unref(person->scripts[type]);
	person->scripts[type] = script;
}
