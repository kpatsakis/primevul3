BOOL IsDeviceName(wchar_t wszVolume[TC_MAX_PATH])
{
	if	(	(wszVolume[0] == '\\')
		&&	(wszVolume[1] == 'D' || wszVolume[1] == 'd')
		&&	(wszVolume[2] == 'E' || wszVolume[2] == 'e')
		&&	(wszVolume[3] == 'V' || wszVolume[3] == 'v')
		&&	(wszVolume[4] == 'I' || wszVolume[4] == 'i')
		&&	(wszVolume[5] == 'C' || wszVolume[5] == 'c')
		&&	(wszVolume[6] == 'E' || wszVolume[6] == 'e')
		)
	{
		return TRUE;
	}
	else
		return FALSE;
}
