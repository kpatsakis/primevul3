static int pgpGet(const uint8_t *s, size_t nbytes, const uint8_t *send,
		  unsigned int *valp)
{
    int rc = -1;

    *valp = 0;
    if (nbytes <= 4 && send - s >= nbytes) {
	unsigned int val = pgpGrab(s, nbytes);
	if (send - s - nbytes >= val) {
	    rc = 0;
	    *valp = val;
	}
    }

    return rc;
}