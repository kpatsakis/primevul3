next_field_w(const wchar_t **wp, const wchar_t **start,
    const wchar_t **end, wchar_t *sep)
{
	/* Skip leading whitespace to find start of field. */
	while (**wp == L' ' || **wp == L'\t' || **wp == L'\n') {
		(*wp)++;
	}
	*start = *wp;

	/* Scan for the separator. */
	while (**wp != L'\0' && **wp != L',' && **wp != L':' &&
	    **wp != L'\n' && **wp != L'#') {
		(*wp)++;
	}
	*sep = **wp;

	/* Locate end of field, trim trailing whitespace if necessary */
	if (*wp == *start) {
		*end = *wp;
	} else {
		*end = *wp - 1;
		while (**end == L' ' || **end == L'\t' || **end == L'\n') {
			(*end)--;
		}
		(*end)++;
	}

	/* Handle in-field comments */
	if (*sep == L'#') {
		while (**wp != L'\0' && **wp != L',' && **wp != L'\n') {
			(*wp)++;
		}
		*sep = **wp;
	}

	/* Adjust scanner location. */
	if (**wp != L'\0')
		(*wp)++;
}
