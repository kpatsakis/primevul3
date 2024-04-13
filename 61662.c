_slurm_authorized_user(uid_t uid)
{
	return ((uid == (uid_t) 0) || (uid == conf->slurm_user_id));
}
