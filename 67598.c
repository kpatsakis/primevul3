void nsc_message_free(NSC_CONTEXT* context, NSC_MESSAGE* message)
{
	BufferPool_Return(context->priv->PlanePool, message->PlaneBuffer);
}
