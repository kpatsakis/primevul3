char * pgpArmorWrap(int atype, const unsigned char * s, size_t ns)
{
    char *buf = NULL, *val = NULL;
    char *enc = rpmBase64Encode(s, ns, -1);
    char *crc = rpmBase64CRC(s, ns);
    const char *valstr = pgpValStr(pgpArmorTbl, atype);

    if (crc != NULL && enc != NULL) {
	rasprintf(&buf, "%s=%s", enc, crc);
    }
    free(crc);
    free(enc);

    rasprintf(&val, "-----BEGIN PGP %s-----\nVersion: rpm-" VERSION " (NSS-3)\n\n"
		    "%s\n-----END PGP %s-----\n",
		    valstr, buf != NULL ? buf : "", valstr);

    free(buf);
    return val;
}