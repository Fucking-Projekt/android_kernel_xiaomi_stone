// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

#ifdef CONFIG_SECURITY_SELINUX
extern void kernel_set_selinux_enforcing(bool enforcing);
#endif

#ifdef CONFIG_SPOOF_LBL
bool femboy_selinux_enabled = false;
bool femboy_bl_enabled = false;
bool femboy_fs_enabled = false;
bool spoof_lbl_activated = false;

static int __init parse_femboy_selinux(char *str)
{
	int val;
	if (kstrtoint(str, 0, &val) == 0)
		femboy_selinux_enabled = !!val;
	else
		femboy_selinux_enabled = true;
	return 1;
}
__setup("femboy.selinux=", parse_femboy_selinux);

static int __init parse_femboy_bl(char *str)
{
	int val;
	if (kstrtoint(str, 0, &val) == 0)
		femboy_bl_enabled = !!val;
	else
		femboy_bl_enabled = true;
	return 1;
}
__setup("femboy.bl=", parse_femboy_bl);

static int __init parse_femboy_fs(char *str)
{
	int val;
	if (kstrtoint(str, 0, &val) == 0)
		femboy_fs_enabled = !!val;
	else
		femboy_fs_enabled = true;
	return 1;
}
__setup("femboy.fs=", parse_femboy_fs);

static ssize_t active_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%d\n", spoof_lbl_activated);
}

static ssize_t active_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int new_value;
	if (kstrtoint(buf, 10, &new_value))
		return -EINVAL;
	spoof_lbl_activated = !!new_value;
	pr_info("Femboy spoof LBL state changed to %d\n", spoof_lbl_activated);

#ifdef CONFIG_SECURITY_SELINUX
	kernel_set_selinux_enforcing(spoof_lbl_activated);
#endif

	return count;
}

static struct kobj_attribute active_attribute = __ATTR_RW(active);

static void spoof_lbl_cmdline(char *c)
{
	char *s;
	bool spoof_bl = false;
	bool spoof_selinux = false;

	if (femboy_bl_enabled || (femboy_fs_enabled && spoof_lbl_activated))
		spoof_bl = true;

	if (femboy_selinux_enabled || (femboy_fs_enabled && spoof_lbl_activated))
		spoof_selinux = true;

	if (spoof_bl) {
		while ((s = strstr(c, "androidboot.verifiedbootstate=orange")) != NULL)
			memcpy(s + 30, "green ", 6);

		while ((s = strstr(c, "androidboot.vbmeta.device_state=unlocked")) != NULL)
			memcpy(s + 32, "locked  ", 8);
	}

	if (spoof_selinux) {
		while ((s = strstr(c, "androidboot.selinux=permissive")) != NULL)
			memcpy(s + 20, "enforcing ", 10);
	}
}
#endif

static int cmdline_proc_show(struct seq_file *m, void *v)
{
	char *c = kstrdup(saved_command_line, GFP_KERNEL);
	if (!c) {
		seq_puts(m, saved_command_line);
		seq_putc(m, '\n');
		return 0;
	}

#ifdef CONFIG_SPOOF_LBL
	spoof_lbl_cmdline(c);
#endif

#ifdef CONFIG_HQ_SYSFS_SUPPORT
	{
		extern const char *get_huaqin_pcba_string(void);
		const char *pcba_raw = get_huaqin_pcba_string();
		char project[16] = {0};
		char stage[16] = {0};
		char region[16] = {0};
		char hwc[16] = {0};
		char hwlevel[16] = {0};
		char batch[16] = {0};
		char tiny[16] = {0};
		char rev[16] = {0};
		char modelcert[32] = {0};

		if (pcba_raw && strncmp(pcba_raw, "PCBA_", 5) == 0) {
			char temp[64];
			char *p, *tok;
			int idx = 0;

			strlcpy(temp, pcba_raw, sizeof(temp));
			p = temp;
			while ((tok = strsep(&p, "_")) != NULL) {
				if (idx == 1) strlcpy(project, tok, sizeof(project));
				else if (idx == 2) strlcpy(stage, tok, sizeof(stage));
				else if (idx == 3) strlcpy(region, tok, sizeof(region));
				else if (idx == 4 && strcmp(stage, "P0") == 0) {
					strlcpy(stage, "P0_1", sizeof(stage));
					strlcpy(region, tok, sizeof(region));
				}
				idx++;
			}

			if (strcmp(stage, "MP") == 0 || strcmp(stage, "PREM") == 0) {
				strcpy(hwlevel, "MP"); strcpy(rev, "0");
			} else if (strncmp(stage, "P0", 2) == 0) {
				strcpy(hwlevel, "P0"); strcpy(rev, "1");
			} else if (strcmp(stage, "P1") == 0) {
				strcpy(hwlevel, "P1"); strcpy(rev, "1");
			} else if (strcmp(stage, "P2") == 0) {
				strcpy(hwlevel, "P2"); strcpy(rev, "2");
			} else {
				strcpy(hwlevel, "MP"); strcpy(rev, "0");
			}

			if (strcmp(project, "M17P") == 0) {
				if (strcmp(region, "IN") == 0) {
					strcpy(hwc, "IN"); strcpy(batch, "3563B"); strcpy(tiny, "19"); strcpy(modelcert, "22111317PI");
				} else if (strcmp(region, "GL") == 0 || strcmp(region, "GLOBAL") == 0) {
					strcpy(hwc, "GLOBAL"); strcpy(batch, "6335B"); strcpy(tiny, "19"); strcpy(modelcert, "22111317PG");
				} else {
					strcpy(hwc, "GLOBAL"); strcpy(batch, "6335B"); strcpy(tiny, "19"); strcpy(modelcert, "22111317PG");
				}
			} else if (strcmp(project, "M17") == 0) {
				if (strcmp(region, "IN") == 0) {
					strcpy(hwc, "IN"); strcpy(batch, "3563"); strcpy(tiny, "17"); strcpy(modelcert, "22111317I");
				} else if (strcmp(region, "GL") == 0 || strcmp(region, "GLOBAL") == 0) {
					strcpy(hwc, "GLOBAL"); strcpy(batch, "3563"); strcpy(tiny, "17"); strcpy(modelcert, "22111317G");
				} else if (strcmp(region, "CN") == 0) {
					strcpy(hwc, "CN"); strcpy(batch, "3563"); strcpy(tiny, "17"); strcpy(modelcert, "22101317C");
				} else {
					strcpy(hwc, "GLOBAL"); strcpy(batch, "3563"); strcpy(tiny, "17"); strcpy(modelcert, "22111317G");
				}
			} else if (strcmp(project, "M17X") == 0) {
				strcpy(hwc, "CN"); strcpy(batch, "3563"); strcpy(tiny, "17"); strcpy(modelcert, "22101317C");
			} else if (strcmp(project, "K19J") == 0 || strcmp(project, "K19K") == 0) {
				if (strcmp(region, "JP") == 0) {
					strcpy(hwc, "JP"); strcpy(batch, "9KJa"); strcpy(tiny, "19"); strcpy(modelcert, "22021119KR");
				} else {
					strcpy(hwc, "GLOBAL"); strcpy(batch, "1835"); strcpy(tiny, "18"); strcpy(modelcert, "22021119G");
				}
			} else {
				strcpy(hwc, "GLOBAL"); strcpy(batch, "6335B"); strcpy(tiny, "19"); strcpy(modelcert, "22111317PG");
			}

			seq_printf(m, "%s androidboot.hwc=%s androidboot.hwlevel=%s androidboot.hwversion=%s.%s.%s androidboot.modelcert=%s\n",
				c, hwc, hwlevel, batch, tiny, rev, modelcert);
			kfree(c);
			return 0;
		}
	}
#endif
	seq_printf(m, "%s\n", c);
	kfree(c);
	return 0;
}

static int __init proc_cmdline_init(void)
{
#ifdef CONFIG_SPOOF_LBL
	int retval;
	struct kobject *femboy_kobj;
#endif

	proc_create_single("cmdline", 0, NULL, cmdline_proc_show);

#ifdef CONFIG_SPOOF_LBL
	if (femboy_fs_enabled) {
		femboy_kobj = kobject_create_and_add("femboy", kernel_kobj);
		if (femboy_kobj) {
			retval = sysfs_create_file(femboy_kobj, &active_attribute.attr);
			if (retval)
				pr_err("femboy: failed to create active sysfs node\n");
		}
	} else {
#ifdef CONFIG_SECURITY_SELINUX
		if (femboy_selinux_enabled) {
			kernel_set_selinux_enforcing(true);
		}
#endif
	}
#endif

	return 0;
}
fs_initcall(proc_cmdline_init);
