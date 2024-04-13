update_max_tr(struct trace_array *tr, struct task_struct *tsk, int cpu)
{
	if (tr->stop_count)
		return;

	WARN_ON_ONCE(!irqs_disabled());

	if (!tr->allocated_snapshot) {
		/* Only the nop tracer should hit this when disabling */
		WARN_ON_ONCE(tr->current_trace != &nop_trace);
		return;
	}

	arch_spin_lock(&tr->max_lock);

	/* Inherit the recordable setting from trace_buffer */
	if (ring_buffer_record_is_set_on(tr->trace_buffer.buffer))
		ring_buffer_record_on(tr->max_buffer.buffer);
	else
		ring_buffer_record_off(tr->max_buffer.buffer);

	swap(tr->trace_buffer.buffer, tr->max_buffer.buffer);

	__update_max_tr(tr, tsk, cpu);
	arch_spin_unlock(&tr->max_lock);
}
