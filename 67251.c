static void free_String(String* string) {
	if (string) {
		free (string->szKey);
		free (string->Value);
		free (string);
	}
}
