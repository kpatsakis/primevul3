BOOL IsAllZeroes (unsigned char* pbData, DWORD dwDataLen)
{
	while (dwDataLen--)
	{
		if (*pbData)
			return FALSE;
		pbData++;
	}
	return TRUE;
}
