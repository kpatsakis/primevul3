int ratestrtosize(char *s, uint_fast32_t rawsize, uint_fast32_t *size)
{
	jpc_flt_t f;

	/* Note: This function must not modify output size on failure. */
	if (strchr(s, 'B')) {
		*size = atoi(s);
	} else {
		f = atof(s);
		if (f < 0) {
			*size = 0;
		} else if (f > 1.0) {
			*size = rawsize + 1;
		} else {
			*size = f * rawsize;
		}
	}
	return 0;
}
