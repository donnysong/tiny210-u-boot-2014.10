dtb-$(CONFIG_S5PC100) += s5pc1xx-smdkc100.dtb
dtb-$(CONFIG_S5PC110) += s5pc1xx-goni.dtb
dtb-y += s5p210-tiny210.dtb
dtb-$(CONFIG_EXYNOS4) += exynos4210-origen.dtb \
	exynos4210-smdkv310.dtb \
	exynos4210-universal_c210.dtb \
	exynos4210-trats.dtb \
	exynos4412-trats2.dtb \
	exynos4412-odroid.dtb

dtb-$(CONFIG_EXYNOS5) += exynos5250-arndale.dtb \
	exynos5250-snow.dtb \
	exynos5250-smdk5250.dtb \
	exynos5420-smdk5420.dtb \
	exynos5420-peach-pit.dtb
dtb-$(CONFIG_TEGRA) += tegra20-harmony.dtb \
	tegra20-medcom-wide.dtb \
	tegra20-paz00.dtb \
	tegra20-plutux.dtb \
	tegra20-seaboard.dtb \
	tegra20-tec.dtb \
	tegra20-trimslice.dtb \
	tegra20-ventana.dtb \
	tegra20-whistler.dtb \
	tegra20-colibri_t20_iris.dtb \
	tegra30-beaver.dtb \
	tegra30-cardhu.dtb \
	tegra30-colibri.dtb \
	tegra30-tec-ng.dtb \
	tegra114-dalmore.dtb \
	tegra124-jetson-tk1.dtb \
	tegra124-venice2.dtb
dtb-$(CONFIG_ZYNQ) += zynq-zc702.dtb \
	zynq-zc706.dtb \
	zynq-zed.dtb \
	zynq-microzed.dtb \
	zynq-zc770-xm010.dtb \
	zynq-zc770-xm012.dtb \
	zynq-zc770-xm013.dtb
dtb-$(CONFIG_AM33XX) += am335x-boneblack.dtb

targets += $(dtb-y)

DTC_FLAGS += -R 4 -p 0x1000

PHONY += dtbs
dtbs: $(addprefix $(obj)/, $(dtb-y))
	@:

clean-files := *.dtb
