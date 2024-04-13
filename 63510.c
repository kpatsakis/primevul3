static inline struct user_namespace *to_user_ns(struct ns_common *ns)
{
	return container_of(ns, struct user_namespace, ns);
}
