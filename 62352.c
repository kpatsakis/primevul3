static int einj_get_available_error_type(u32 *type)
{
	int rc;

	mutex_lock(&einj_mutex);
	rc = __einj_get_available_error_type(type);
	mutex_unlock(&einj_mutex);

	return rc;
}
