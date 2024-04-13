static const char * pgpValStr(pgpValTbl vs, uint8_t val)
{
    do {
	if (vs->val == val)
	    break;
    } while ((++vs)->val != -1);
    return vs->str;
}