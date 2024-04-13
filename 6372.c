eat_sql_whitespace(const char *query)
{
	bool comment = false;

	while (*query) {
		if (comment) {
			if (query[0] == '*' && query[1] == '/') {
				query += 2;
				comment = false;
				continue;
			}
			++query;
			continue;
		}
		if (query[0] == '/' && query[1] == '*') {
			query += 2;
			comment = true;
			continue;
		}
		if (strchr("\t\n\r (", query[0])) {
			++query;
			continue;
		}
		break;
	}

	return(query);
}