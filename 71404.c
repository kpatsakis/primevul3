static void ib_uverbs_release_async_event_file(struct kref *ref)
{
	struct ib_uverbs_async_event_file *file =
		container_of(ref, struct ib_uverbs_async_event_file, ref);

	kfree(file);
}
