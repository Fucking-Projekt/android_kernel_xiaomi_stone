#include <linux/module.h>
#include <linux/device.h>
#include <linux/soc/qcom/smem.h>
#include <linux/soc/qcom/smem_type.h>
#include <misc/hqsys_pcba.h>

static PCBA_CONFIG huaqin_pcba_config = PCBA_UNKNOW;

PCBA_CONFIG get_huaqin_pcba_config(void)
{
	PCBA_CONFIG *pcba_config = NULL;
	size_t size;

	pcba_config = (PCBA_CONFIG *)qcom_smem_get(QCOM_SMEM_HOST_ANY, SMEM_ID_VENDOR1, &size);
	if (pcba_config) {
		pr_err("pcba config = %d 0x%x.\n", *(pcba_config), *(pcba_config));
		if (*(pcba_config) > PCBA_UNKNOW && *(pcba_config) < PCBA_END) {
			huaqin_pcba_config = *pcba_config;
		} else {
			huaqin_pcba_config = PCBA_UNKNOW;
		}
	} else {
		pr_err("pcba config failed\n");
		huaqin_pcba_config = PCBA_UNKNOW;
	}
	return huaqin_pcba_config;
}
EXPORT_SYMBOL_GPL(get_huaqin_pcba_config);

static int __init huaqin_pcba_early_init(void)
{
	get_huaqin_pcba_config();
	return 0;
}

subsys_initcall(huaqin_pcba_early_init); //before device_initcall

//late_initcall(huaqin_pcba_early_init);   //late initcall

MODULE_AUTHOR("lizheng<LiZheng6@huaqin.com>");
MODULE_DESCRIPTION("huaqin sys pcba");
MODULE_LICENSE("GPL");
