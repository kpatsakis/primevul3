void RootDeviceControlMutexRelease ()
{
	KeReleaseMutex (&RootDeviceControlMutex, FALSE);
}
