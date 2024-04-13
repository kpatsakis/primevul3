int __kvm_set_memory_region(struct kvm *kvm,
			    const struct kvm_userspace_memory_region *mem)
{
	int r;
	gfn_t base_gfn;
	unsigned long npages;
	struct kvm_memory_slot *slot;
	struct kvm_memory_slot old, new;
	struct kvm_memslots *slots = NULL, *old_memslots;
	int as_id, id;
	enum kvm_mr_change change;

	r = check_memory_region_flags(mem);
	if (r)
		goto out;

	r = -EINVAL;
	as_id = mem->slot >> 16;
	id = (u16)mem->slot;

	/* General sanity checks */
	if (mem->memory_size & (PAGE_SIZE - 1))
		goto out;
	if (mem->guest_phys_addr & (PAGE_SIZE - 1))
		goto out;
	/* We can read the guest memory with __xxx_user() later on. */
	if ((id < KVM_USER_MEM_SLOTS) &&
	    ((mem->userspace_addr & (PAGE_SIZE - 1)) ||
	     !access_ok(VERIFY_WRITE,
			(void __user *)(unsigned long)mem->userspace_addr,
			mem->memory_size)))
		goto out;
	if (as_id >= KVM_ADDRESS_SPACE_NUM || id >= KVM_MEM_SLOTS_NUM)
		goto out;
	if (mem->guest_phys_addr + mem->memory_size < mem->guest_phys_addr)
		goto out;

	slot = id_to_memslot(__kvm_memslots(kvm, as_id), id);
	base_gfn = mem->guest_phys_addr >> PAGE_SHIFT;
	npages = mem->memory_size >> PAGE_SHIFT;

	if (npages > KVM_MEM_MAX_NR_PAGES)
		goto out;

	new = old = *slot;

	new.id = id;
	new.base_gfn = base_gfn;
	new.npages = npages;
	new.flags = mem->flags;

	if (npages) {
		if (!old.npages)
			change = KVM_MR_CREATE;
		else { /* Modify an existing slot. */
			if ((mem->userspace_addr != old.userspace_addr) ||
			    (npages != old.npages) ||
			    ((new.flags ^ old.flags) & KVM_MEM_READONLY))
				goto out;

			if (base_gfn != old.base_gfn)
				change = KVM_MR_MOVE;
			else if (new.flags != old.flags)
				change = KVM_MR_FLAGS_ONLY;
			else { /* Nothing to change. */
				r = 0;
				goto out;
			}
		}
	} else {
		if (!old.npages)
			goto out;

		change = KVM_MR_DELETE;
		new.base_gfn = 0;
		new.flags = 0;
	}

	if ((change == KVM_MR_CREATE) || (change == KVM_MR_MOVE)) {
		/* Check for overlaps */
		r = -EEXIST;
		kvm_for_each_memslot(slot, __kvm_memslots(kvm, as_id)) {
			if ((slot->id >= KVM_USER_MEM_SLOTS) ||
			    (slot->id == id))
				continue;
			if (!((base_gfn + npages <= slot->base_gfn) ||
			      (base_gfn >= slot->base_gfn + slot->npages)))
				goto out;
		}
	}

	/* Free page dirty bitmap if unneeded */
	if (!(new.flags & KVM_MEM_LOG_DIRTY_PAGES))
		new.dirty_bitmap = NULL;

	r = -ENOMEM;
	if (change == KVM_MR_CREATE) {
		new.userspace_addr = mem->userspace_addr;

		if (kvm_arch_create_memslot(kvm, &new, npages))
			goto out_free;
	}

	/* Allocate page dirty bitmap if needed */
	if ((new.flags & KVM_MEM_LOG_DIRTY_PAGES) && !new.dirty_bitmap) {
		if (kvm_create_dirty_bitmap(&new) < 0)
			goto out_free;
	}

	slots = kvm_kvzalloc(sizeof(struct kvm_memslots));
	if (!slots)
		goto out_free;
	memcpy(slots, __kvm_memslots(kvm, as_id), sizeof(struct kvm_memslots));

	if ((change == KVM_MR_DELETE) || (change == KVM_MR_MOVE)) {
		slot = id_to_memslot(slots, id);
		slot->flags |= KVM_MEMSLOT_INVALID;

		old_memslots = install_new_memslots(kvm, as_id, slots);

		/* slot was deleted or moved, clear iommu mapping */
		kvm_iommu_unmap_pages(kvm, &old);
		/* From this point no new shadow pages pointing to a deleted,
		 * or moved, memslot will be created.
		 *
		 * validation of sp->gfn happens in:
		 *	- gfn_to_hva (kvm_read_guest, gfn_to_pfn)
		 *	- kvm_is_visible_gfn (mmu_check_roots)
		 */
		kvm_arch_flush_shadow_memslot(kvm, slot);

		/*
		 * We can re-use the old_memslots from above, the only difference
		 * from the currently installed memslots is the invalid flag.  This
		 * will get overwritten by update_memslots anyway.
		 */
		slots = old_memslots;
	}

	r = kvm_arch_prepare_memory_region(kvm, &new, mem, change);
	if (r)
		goto out_slots;

	/* actual memory is freed via old in kvm_free_memslot below */
	if (change == KVM_MR_DELETE) {
		new.dirty_bitmap = NULL;
		memset(&new.arch, 0, sizeof(new.arch));
	}

	update_memslots(slots, &new);
	old_memslots = install_new_memslots(kvm, as_id, slots);

	kvm_arch_commit_memory_region(kvm, mem, &old, &new, change);

	kvm_free_memslot(kvm, &old, &new);
	kvfree(old_memslots);

	/*
	 * IOMMU mapping:  New slots need to be mapped.  Old slots need to be
	 * un-mapped and re-mapped if their base changes.  Since base change
	 * unmapping is handled above with slot deletion, mapping alone is
	 * needed here.  Anything else the iommu might care about for existing
	 * slots (size changes, userspace addr changes and read-only flag
	 * changes) is disallowed above, so any other attribute changes getting
	 * here can be skipped.
	 */
	if ((change == KVM_MR_CREATE) || (change == KVM_MR_MOVE)) {
		r = kvm_iommu_map_pages(kvm, &new);
		return r;
	}

	return 0;

out_slots:
	kvfree(slots);
out_free:
	kvm_free_memslot(kvm, &new, &old);
out:
	return r;
}
