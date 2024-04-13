person_compile_script(person_t* person, int type, const lstring_t* codestring)
{
	script_t*   script;
	const char* script_name;

	script_name = type == PERSON_SCRIPT_ON_CREATE ? "onCreate"
		: type == PERSON_SCRIPT_ON_DESTROY ? "onDestroy"
		: type == PERSON_SCRIPT_ON_TOUCH ? "onTouch"
		: type == PERSON_SCRIPT_ON_TALK ? "onTalk"
		: type == PERSON_SCRIPT_GENERATOR ? "genCommands"
		: NULL;
	if (script_name == NULL)
		return false;
	script = script_new(codestring, "%s/%s/%s.js", map_pathname(), person->name, script_name);
	person_on_event(person, type, script);
	return true;
}
