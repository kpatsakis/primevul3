free_mysql_variables(mysql_variable *vars)
{
	mysql_variable *var;

	for (var = vars; var->name; var++) {
		free(*(var->value));
		*var->value = NULL;
	}
}