bsdtar_expand_char(char *buff, size_t offset, char c)
{
	size_t i = offset;

	if (isprint((unsigned char)c) && c != '\\')
		buff[i++] = c;
	else {
		buff[i++] = '\\';
		switch (c) {
		case '\a': buff[i++] = 'a'; break;
		case '\b': buff[i++] = 'b'; break;
		case '\f': buff[i++] = 'f'; break;
		case '\n': buff[i++] = 'n'; break;
#if '\r' != '\n'
		/* On some platforms, \n and \r are the same. */
		case '\r': buff[i++] = 'r'; break;
#endif
		case '\t': buff[i++] = 't'; break;
		case '\v': buff[i++] = 'v'; break;
		case '\\': buff[i++] = '\\'; break;
		default:
			sprintf(buff + i, "%03o", 0xFF & (int)c);
			i += 3;
		}
	}

	return (i - offset);
}
