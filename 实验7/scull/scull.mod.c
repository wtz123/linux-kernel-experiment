#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xede291fa, "module_layout" },
	{ 0xfeb652a5, "cdev_del" },
	{ 0x11ecb55e, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x9f63b808, "cdev_init" },
	{ 0x6980fe91, "param_get_int" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x8d3d0fc1, "no_llseek" },
	{ 0x4aabc7c4, "__tracepoint_kmalloc" },
	{ 0x670c0597, "down_interruptible" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xff964b25, "param_set_int" },
	{ 0x5cfa6964, "nonseekable_open" },
	{ 0xe174aa7, "__init_waitqueue_head" },
	{ 0x2bc95bd4, "memset" },
	{ 0xa2aadc44, "kmem_cache_alloc_notrace" },
	{ 0x5ad9bcfd, "current_task" },
	{ 0xb72397d5, "printk" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x8b630380, "fasync_helper" },
	{ 0xb4390f9a, "mcount" },
	{ 0x1a925a66, "down" },
	{ 0x31f72708, "cdev_add" },
	{ 0x7dceceac, "capable" },
	{ 0xb2fd5ceb, "__put_user_4" },
	{ 0x4292364c, "schedule" },
	{ 0xf09c7f68, "__wake_up" },
	{ 0x37a0cba, "kfree" },
	{ 0xe75663a, "prepare_to_wait" },
	{ 0x57b09822, "up" },
	{ 0x238807dc, "kill_fasync" },
	{ 0xb00ccc33, "finish_wait" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "F9B691CDD6EBCA66CDD9EA5");
