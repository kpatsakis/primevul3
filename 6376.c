get_table_name_from_tablespace(std::string &tablespace)
{
	std::size_t pos = tablespace.find("`.`"); //`db`.`table` separator
	if (pos != std::string::npos) {
		tablespace = tablespace.substr(pos+3);
		tablespace.erase(tablespace.size()-1); //remove leading `
	}
}