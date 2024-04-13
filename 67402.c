struct sighand_struct *__lock_task_sighand(struct task_struct *tsk,
					   unsigned long *flags)
{
	struct sighand_struct *sighand;

	for (;;) {
		/*
		 * Disable interrupts early to avoid deadlocks.
		 * See rcu_read_unlock() comment header for details.
		 */
		local_irq_save(*flags);
		rcu_read_lock();
		sighand = rcu_dereference(tsk->sighand);
		if (unlikely(sighand == NULL)) {
			rcu_read_unlock();
			local_irq_restore(*flags);
			break;
		}
		/*
		 * This sighand can be already freed and even reused, but
		 * we rely on SLAB_TYPESAFE_BY_RCU and sighand_ctor() which
		 * initializes ->siglock: this slab can't go away, it has
		 * the same object type, ->siglock can't be reinitialized.
		 *
		 * We need to ensure that tsk->sighand is still the same
		 * after we take the lock, we can race with de_thread() or
		 * __exit_signal(). In the latter case the next iteration
		 * must see ->sighand == NULL.
		 */
		spin_lock(&sighand->siglock);
		if (likely(sighand == tsk->sighand)) {
			rcu_read_unlock();
			break;
		}
		spin_unlock(&sighand->siglock);
		rcu_read_unlock();
		local_irq_restore(*flags);
	}

	return sighand;
}
