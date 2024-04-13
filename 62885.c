isint_w(const wchar_t *start, const wchar_t *end, int *result)
{
	int n = 0;
	if (start >= end)
		return (0);
	while (start < end) {
		if (*start < '0' || *start > '9')
			return (0);
		if (n > (INT_MAX / 10) ||
		    (n == INT_MAX / 10 && (*start - '0') > INT_MAX % 10)) {
			n = INT_MAX;
		} else {
			n *= 10;
			n += *start - '0';
		}
		start++;
	}
	*result = n;
	return (1);
}
