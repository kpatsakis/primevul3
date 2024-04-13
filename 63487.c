void __put_user_ns(struct user_namespace *ns)
{
	schedule_work(&ns->work);
}
