is_select_query(const char *query)
{
	const char *query_list[] = {"select", NULL};

	return is_query_from_list(query, query_list);
}