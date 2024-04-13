read_mysql_variables_from_result(MYSQL_RES *mysql_result, mysql_variable *vars,
	bool vertical_result)
{
	MYSQL_ROW row;
	mysql_variable *var;
	ut_ad(!vertical_result || mysql_num_fields(mysql_result) == 2);
	int rows_read = 0;

	if (vertical_result) {
		while ((row = mysql_fetch_row(mysql_result))) {
			++rows_read;
			char *name = row[0];
			char *value = row[1];
			for (var = vars; var->name; var++) {
				if (strcmp(var->name, name) == 0
				    && value != NULL) {
					*(var->value) = strdup(value);
				}
			}
		}
	} else {
		MYSQL_FIELD *field;

		if ((row = mysql_fetch_row(mysql_result)) != NULL) {
			mysql_field_seek(mysql_result, 0);
			++rows_read;
			int i = 0;
			while ((field = mysql_fetch_field(mysql_result))
				!= NULL) {
				char *name = field->name;
				char *value = row[i];
				for (var = vars; var->name; var++) {
					if (strcmp(var->name, name) == 0
					    && value != NULL) {
						*(var->value) = strdup(value);
					}
				}
				++i;
			}
		}
	}
	return rows_read;
}