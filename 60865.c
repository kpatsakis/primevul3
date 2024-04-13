static struct kvm *kvm_create_vm(unsigned long type)
{
	int r, i;
	struct kvm *kvm = kvm_arch_alloc_vm();

	if (!kvm)
		return ERR_PTR(-ENOMEM);

	spin_lock_init(&kvm->mmu_lock);
	atomic_inc(&current->mm->mm_count);
	kvm->mm = current->mm;
	kvm_eventfd_init(kvm);
	mutex_init(&kvm->lock);
	mutex_init(&kvm->irq_lock);
	mutex_init(&kvm->slots_lock);
	atomic_set(&kvm->users_count, 1);
	INIT_LIST_HEAD(&kvm->devices);

	r = kvm_arch_init_vm(kvm, type);
	if (r)
		goto out_err_no_disable;

	r = hardware_enable_all();
	if (r)
		goto out_err_no_disable;

#ifdef CONFIG_HAVE_KVM_IRQFD
	INIT_HLIST_HEAD(&kvm->irq_ack_notifier_list);
#endif

	BUILD_BUG_ON(KVM_MEM_SLOTS_NUM > SHRT_MAX);

	r = -ENOMEM;
	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++) {
		kvm->memslots[i] = kvm_alloc_memslots();
		if (!kvm->memslots[i])
			goto out_err_no_srcu;
	}

	if (init_srcu_struct(&kvm->srcu))
		goto out_err_no_srcu;
	if (init_srcu_struct(&kvm->irq_srcu))
		goto out_err_no_irq_srcu;
	for (i = 0; i < KVM_NR_BUSES; i++) {
		kvm->buses[i] = kzalloc(sizeof(struct kvm_io_bus),
					GFP_KERNEL);
		if (!kvm->buses[i])
			goto out_err;
	}

	r = kvm_init_mmu_notifier(kvm);
	if (r)
		goto out_err;

	spin_lock(&kvm_lock);
	list_add(&kvm->vm_list, &vm_list);
	spin_unlock(&kvm_lock);

	preempt_notifier_inc();

	return kvm;

out_err:
	cleanup_srcu_struct(&kvm->irq_srcu);
out_err_no_irq_srcu:
	cleanup_srcu_struct(&kvm->srcu);
out_err_no_srcu:
	hardware_disable_all();
out_err_no_disable:
	for (i = 0; i < KVM_NR_BUSES; i++)
		kfree(kvm->buses[i]);
	for (i = 0; i < KVM_ADDRESS_SPACE_NUM; i++)
		kvm_free_memslots(kvm, kvm->memslots[i]);
	kvm_arch_free_vm(kvm);
	mmdrop(current->mm);
	return ERR_PTR(r);
}
