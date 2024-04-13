void EnsureNullTerminatedString (wchar_t *str, size_t maxSizeInBytes)
{
	ASSERT ((maxSizeInBytes & 1) == 0);
	str[maxSizeInBytes / sizeof (wchar_t) - 1] = 0;
}
