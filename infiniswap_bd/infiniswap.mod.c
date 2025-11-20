#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x89c51293, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x42b6546c, __VMLINUX_SYMBOL_STR(fs_bio_set) },
	{ 0xe31697f9, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x268752c1, __VMLINUX_SYMBOL_STR(alloc_disk) },
	{ 0xaef79280, __VMLINUX_SYMBOL_STR(blk_cleanup_queue) },
	{ 0x45c59759, __VMLINUX_SYMBOL_STR(alloc_disk_node) },
	{ 0xda3e43d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0x784213a6, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0xc6f5de7f, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xaa383b89, __VMLINUX_SYMBOL_STR(ib_dealloc_pd) },
	{ 0x1d810e65, __VMLINUX_SYMBOL_STR(blk_queue_max_hw_sectors) },
	{ 0x94ed34a3, __VMLINUX_SYMBOL_STR(cpu_online_mask) },
	{ 0x79aa04a2, __VMLINUX_SYMBOL_STR(get_random_bytes) },
	{ 0x80d8ed32, __VMLINUX_SYMBOL_STR(blk_mq_start_request) },
	{ 0x84368bed, __VMLINUX_SYMBOL_STR(__tracepoint_block_bio_remap) },
	{ 0x6c88e7a1, __VMLINUX_SYMBOL_STR(ib_destroy_qp) },
	{ 0xdf566a59, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_r9) },
	{ 0x44b1d426, __VMLINUX_SYMBOL_STR(__dynamic_pr_debug) },
	{ 0x91b95795, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x85df9b6c, __VMLINUX_SYMBOL_STR(strsep) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0x7d10c0f, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0xd2cb95aa, __VMLINUX_SYMBOL_STR(lookup_bdev) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0xea6d9a67, __VMLINUX_SYMBOL_STR(rdma_connect) },
	{ 0xcbb152d6, __VMLINUX_SYMBOL_STR(blk_mq_init_queue) },
	{ 0xf432dd3d, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xfe7c4287, __VMLINUX_SYMBOL_STR(nr_cpu_ids) },
	{ 0x43c392b2, __VMLINUX_SYMBOL_STR(ib_alloc_pd) },
	{ 0x7e526bfa, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_r10) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x52305d62, __VMLINUX_SYMBOL_STR(blk_alloc_queue) },
	{ 0x8f64aa4, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0xa1afc789, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0xd1d50bb5, __VMLINUX_SYMBOL_STR(ib_get_dma_mr) },
	{ 0xe1e96c66, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0xf4a73b97, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x427f8983, __VMLINUX_SYMBOL_STR(del_gendisk) },
	{ 0xc4763d22, __VMLINUX_SYMBOL_STR(blk_mq_free_tag_set) },
	{ 0x4c9d28b0, __VMLINUX_SYMBOL_STR(phys_base) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x7ba1676c, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xadb85e39, __VMLINUX_SYMBOL_STR(config_group_init_type_name) },
	{ 0x71a50dbc, __VMLINUX_SYMBOL_STR(register_blkdev) },
	{ 0xa00ef6b0, __VMLINUX_SYMBOL_STR(generic_make_request) },
	{ 0xd8d5166e, __VMLINUX_SYMBOL_STR(configfs_register_subsystem) },
	{ 0xd7a49865, __VMLINUX_SYMBOL_STR(init_net) },
	{ 0x71107a00, __VMLINUX_SYMBOL_STR(rdma_create_id) },
	{ 0x405ff30d, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0x555a0cd7, __VMLINUX_SYMBOL_STR(ib_destroy_cq) },
	{ 0x61651be, __VMLINUX_SYMBOL_STR(strcat) },
	{ 0xb5a459dc, __VMLINUX_SYMBOL_STR(unregister_blkdev) },
	{ 0x67d6ac90, __VMLINUX_SYMBOL_STR(rdma_create_qp) },
	{ 0xafb55d39, __VMLINUX_SYMBOL_STR(rdma_resolve_route) },
	{ 0x78764f4e, __VMLINUX_SYMBOL_STR(pv_irq_ops) },
	{ 0xb601be4c, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_rdx) },
	{ 0x76da64db, __VMLINUX_SYMBOL_STR(blkdev_put) },
	{ 0x2fbd5f73, __VMLINUX_SYMBOL_STR(blk_queue_make_request) },
	{ 0x1122a883, __VMLINUX_SYMBOL_STR(blk_queue_physical_block_size) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0xaccabc6a, __VMLINUX_SYMBOL_STR(in4_pton) },
	{ 0xda157909, __VMLINUX_SYMBOL_STR(put_disk) },
	{ 0x43261dca, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irq) },
	{ 0x2ea2c95c, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_rax) },
	{ 0xbfdcb43a, __VMLINUX_SYMBOL_STR(__x86_indirect_thunk_r11) },
	{ 0x34d8fb19, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0x1bfea536, __VMLINUX_SYMBOL_STR(blk_mq_alloc_tag_set) },
	{ 0xd68d3a2d, __VMLINUX_SYMBOL_STR(configfs_unregister_subsystem) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x386d8fc2, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xd52bf1ce, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x5644652c, __VMLINUX_SYMBOL_STR(bio_clone_bioset) },
	{ 0x9327f5ce, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0xccf4048, __VMLINUX_SYMBOL_STR(blk_mq_end_request) },
	{ 0x912892f7, __VMLINUX_SYMBOL_STR(ib_dereg_mr) },
	{ 0xcf21d241, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0x34f22f94, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x993c4b8f, __VMLINUX_SYMBOL_STR(add_disk) },
	{ 0xd3719d59, __VMLINUX_SYMBOL_STR(paravirt_ticketlocks_enabled) },
	{ 0x786651ca, __VMLINUX_SYMBOL_STR(set_user_nice) },
	{ 0xf4c3a9e, __VMLINUX_SYMBOL_STR(rdma_resolve_addr) },
	{ 0xfa66f77c, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x63c4d61f, __VMLINUX_SYMBOL_STR(__bitmap_weight) },
	{ 0x52398572, __VMLINUX_SYMBOL_STR(ib_create_cq) },
	{ 0x4b06d2e7, __VMLINUX_SYMBOL_STR(complete) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xf5e93dc0, __VMLINUX_SYMBOL_STR(bdget) },
	{ 0xe5792b6d, __VMLINUX_SYMBOL_STR(blkdev_get) },
	{ 0x76f67f1d, __VMLINUX_SYMBOL_STR(blk_queue_logical_block_size) },
	{ 0x872ea9d0, __VMLINUX_SYMBOL_STR(config_group_init) },
	{ 0x17635ba5, __VMLINUX_SYMBOL_STR(blk_mq_map_queue) },
	{ 0xdbf053fe, __VMLINUX_SYMBOL_STR(bdput) },
	{ 0xb24d80c5, __VMLINUX_SYMBOL_STR(dma_ops) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=ib_core,rdma_cm,configfs";


MODULE_INFO(srcversion, "0001684C4FF4FAF06251AC6");
