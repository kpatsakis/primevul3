smb2_dir_needs_close(struct cifsFileInfo *cfile)
{
	return !cfile->invalidHandle;
}