BOOL IsVolumeAccessibleByCurrentUser (PEXTENSION volumeDeviceExtension)
{
	SECURITY_SUBJECT_CONTEXT subContext;
	PACCESS_TOKEN accessToken;
	PTOKEN_USER tokenUser;
	BOOL result = FALSE;

	if (IoIsSystemThread (PsGetCurrentThread())
		|| UserCanAccessDriveDevice()
		|| !volumeDeviceExtension->UserSid
		|| (volumeDeviceExtension->SystemFavorite && !NonAdminSystemFavoritesAccessDisabled))
	{
		return TRUE;
	}

	SeCaptureSubjectContext (&subContext);
	SeLockSubjectContext(&subContext);
	if (subContext.ClientToken && subContext.ImpersonationLevel >= SecurityImpersonation)
		accessToken = subContext.ClientToken;
	else
		accessToken = subContext.PrimaryToken;

	if (!accessToken)
		goto ret;

	if (SeTokenIsAdmin (accessToken))
	{
		result = TRUE;
		goto ret;
	}

	if (!NT_SUCCESS (SeQueryInformationToken (accessToken, TokenUser, &tokenUser)))
		goto ret;

	result = RtlEqualSid (volumeDeviceExtension->UserSid, tokenUser->User.Sid);
	ExFreePool (tokenUser);		// Documented in newer versions of WDK

ret:
	SeUnlockSubjectContext(&subContext);
	SeReleaseSubjectContext (&subContext);
	return result;
}
