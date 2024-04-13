static inline int checkSettingSecurity(lua_State* L, const std::string &name)
{
	if (ScriptApiSecurity::isSecure(L) && name.compare(0, 7, "secure.") == 0)
		throw LuaError("Attempt to set secure setting.");

	bool is_mainmenu = false;
#ifndef SERVER
	is_mainmenu = ModApiBase::getGuiEngine(L) != nullptr;
#endif
	if (!is_mainmenu && (name == "mg_name" || name == "mg_flags")) {
		errorstream << "Tried to set global setting " << name << ", ignoring. "
			"minetest.set_mapgen_setting() should be used instead." << std::endl;
		infostream << script_get_backtrace(L) << std::endl;
		return -1;
	}

	return 0;
}