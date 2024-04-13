static void userns_put(struct ns_common *ns)
{
	put_user_ns(to_user_ns(ns));
}
