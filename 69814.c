void TCCloseFsVolume (HANDLE volumeHandle, PFILE_OBJECT fileObject)
{
	ObDereferenceObject (fileObject);
	ZwClose (volumeHandle);
}
