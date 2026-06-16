
#include <soc/qcom/socinfo.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/soc/qcom/smem.h>
//#include <linux/hwid.h>
#include "mi_memory_sysfs.h"
#include "mem_interface.h"
#include <linux/of.h>

#if MEMORYTPYE_DIS
#define SMEM_ID_VENDOR1 	135
#define SMEM_ID_VENDOR2 	136
#endif

u8 get_ddr_size(void)
{
	u8 ddr_size_in_GB = 0;

	ddr_size_in_GB = memblock_mem_size_in_gb();
	pr_err("memblock_mem_size %d\n", ddr_size_in_GB);

	if (ddr_size_in_GB > 16 && ddr_size_in_GB <= 18) {
		ddr_size_in_GB = 18;
	} else if (ddr_size_in_GB >= 12) {
		ddr_size_in_GB = 16;
	} else if (ddr_size_in_GB >= 10) {
		ddr_size_in_GB = 12;
	} else if (ddr_size_in_GB >= 8) {
		ddr_size_in_GB = 10;
	} else if (ddr_size_in_GB >= 6) {
		ddr_size_in_GB = 8;
	} else if (ddr_size_in_GB >= 4) {
		ddr_size_in_GB = 6;
	} else if (ddr_size_in_GB >= 3) {
		ddr_size_in_GB = 4;
	} else if (ddr_size_in_GB >= 2) {
		ddr_size_in_GB = 3;
	} else if (ddr_size_in_GB >= 1) {
		ddr_size_in_GB = 2;
	} else {
		ddr_size_in_GB = 0;
	}
	/*M17-T code for HQ-209102 by gaoshilin at 22.11.11 start*/
	printk("after get_ddr_size ddr_size_in_GB = %d\n", ddr_size_in_GB);
	/*M17-T code for HQ-209102 by gaoshilin at 22.11.11 end*/
	return (u8)ddr_size_in_GB;
}

uint32_t get_ddr_id(void)
{
	u32 ddr_manufacturer_id = 0;
	struct device_node *mem_node;

	mem_node = of_find_node_by_path("/memory");
	if (!mem_node)
		return 0;

	of_property_read_u32(mem_node, "ddr_manufacturer_id", &ddr_manufacturer_id);
	of_node_put(mem_node);

	return ddr_manufacturer_id;
}

void get_ddr_vendor(char *vendor)
{
	uint32_t ddr_id = 0;

	ddr_id = get_ddr_id();

	switch (ddr_id) {
	case HWINFO_DDRID_SAMSUNG:
		memcpy(vendor, "SAMSUNG", sizeof("SAMSUNG"));
		break;
//BSP.memory - 2022.06.30 - Compatible with cxmt --start
	case HWINFO_DDRID_CXMT:
		memcpy(vendor, "CXMT", sizeof("CXMT"));
		break;
//BSP.memory - 2022.06.30 - Compatible with cxmt --end
	case HWINFO_DDRID_HYNIX:
		memcpy(vendor, "HYNIX", sizeof("HYNIX"));
		break;
	case HWINFO_DDRID_ELPIDA:
		memcpy(vendor, "ELPIDA", sizeof("ELPIDA"));
		break;
	case HWINFO_DDRID_MICRON:
		memcpy(vendor, "MICRON", sizeof("MICRON"));
		break;
	case HWINFO_DDRID_NANYA:
		memcpy(vendor, "NANYA", sizeof("NANYA"));
		break;
	case HWINFO_DDRID_INTEL:
		memcpy(vendor, "INTEL", sizeof("INTEL"));
		break;
	default:
		memcpy(vendor, "UNKNOWN", sizeof("UNKNOWN"));
		break;
	}
}

static ssize_t ddr_size_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", get_ddr_size());
}

static DEVICE_ATTR_RO(ddr_size);

static ssize_t ddr_id_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "0x%02x\n", get_ddr_id());
}

static DEVICE_ATTR_RO(ddr_id);

static ssize_t ddr_vendor_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	char vendor[8] = {0};

	get_ddr_vendor((char *)&vendor);

	return snprintf(buf, PAGE_SIZE, "%s\n", vendor);
}

static DEVICE_ATTR_RO(ddr_vendor);

static struct attribute *dram_sysfs[] = {
	&dev_attr_ddr_size.attr,
	&dev_attr_ddr_id.attr,
	&dev_attr_ddr_vendor.attr,
	NULL,
};

const struct attribute_group dram_sysfs_group = {
	.name = "ddr",
	.attrs = dram_sysfs,
};
