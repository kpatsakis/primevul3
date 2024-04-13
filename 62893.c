static void drdynvc_queue_object_free(void* obj)
{
	wStream* s;
	wMessage* msg = (wMessage*)obj;

	if (!msg || (msg->id != 0))
		return;

	s = (wStream*)msg->wParam;

	if (s)
		Stream_Free(s, TRUE);
}
