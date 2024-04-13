static Sdb* Pe_r_bin_store_string(String* string) {
	Sdb* sdb = NULL;
	char* encodedVal = NULL, * encodedKey = NULL;
	if (!string) {
		return NULL;
	}
	sdb = sdb_new0 ();
	if (!sdb) {
		return NULL;
	}
	encodedKey = sdb_encode ((unsigned char*) string->szKey, string->wKeyLen);
	if (!encodedKey) {
		sdb_free (sdb);
		return NULL;
	}
	encodedVal = sdb_encode ((unsigned char*) string->Value, string->wValueLength);
	if (!encodedVal) {
		free (encodedKey);
		sdb_free (sdb);
		return NULL;
	}
	sdb_set (sdb, "key",   encodedKey, 0);
	sdb_set (sdb, "value", encodedVal, 0);
	free (encodedKey);
	free (encodedVal);
	return sdb;
}
