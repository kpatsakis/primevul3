smb2_new_lease_key(struct cifs_fid *fid)
{
	generate_random_uuid(fid->lease_key);
}