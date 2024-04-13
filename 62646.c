enum ovl_path_type ovl_path_real(struct dentry *dentry, struct path *path)
{

	enum ovl_path_type type = ovl_path_type(dentry);

	if (type == OVL_PATH_LOWER)
		ovl_path_lower(dentry, path);
	else
		ovl_path_upper(dentry, path);

	return type;
}
