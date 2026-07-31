// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

static int cmdline_proc_show(struct seq_file *m, void *v)
{
#if defined(CONFIG_HQ_SYSFS_SUPPORT)
	char *c = kstrdup(saved_command_line, GFP_KERNEL);
	if (!c) {
		seq_puts(m, saved_command_line);
		seq_putc(m, '\n');
		return 0;
	}

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
	seq_printf(m, "%s\n", c);
	kfree(c);
	return 0;
#else
	seq_puts(m, saved_command_line);
	seq_putc(m, '\n');
	return 0;
#endif
}

static int __init proc_cmdline_init(void)
{
	proc_create_single("cmdline", 0, NULL, cmdline_proc_show);
	return 0;
}
fs_initcall(proc_cmdline_init);
