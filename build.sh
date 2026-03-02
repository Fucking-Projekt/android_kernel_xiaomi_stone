#!/bin/bash

echo -e "==========================="
echo -e "= START COMPILING KERNEL  ="
echo -e "==========================="
bold=$(tput bold)
normal=$(tput sgr0)

export KBUILD_BUILD_USER="mornye"
export TZ=Asia/Jakarta
export KBUILD_BUILD_VERSION=1
export KBUILD_BUILD_TIMESTAMP="Thu Jan 1 07:00:00 WIB 1970"
export KBUILD_BUILD_HOST="build-host"
export KERNELDIR="$(pwd)"
export KERNELNAME="Sparxie"
export SRCDIR="${KERNELDIR}"
export OUTDIR="${KERNELDIR}/out"
export ANYKERNEL="${KERNELDIR}/AnyKernel3"
export DEFCONFIG="stone_defconfig"
export ZIP_DIR="${KERNELDIR}/files"
export IMAGE="${OUTDIR}/arch/arm64/boot/Image"
export VARI="meme-hos2+"
export PATH="$(pwd)/../clang/bin:$PATH"

while (( ${#} )); do
    case ${1} in
        "-r"|"--regen") REGEN=true ;;
        "-c"|"--clean") CLEAN=true ;;
    esac
    shift
done

[[ -z ${ZIP} ]] && { echo "${bold}LOADING-_-....${normal}"; }

curl -LSs "https://raw.githubusercontent.com/ReSukiSU/ReSukiSU/main/kernel/setup.sh" | bash -s dev

KSU_LINK="drivers/kernelsu"

if [ -d "$KSU_LINK" ]; then
    echo "[*] Silent exessive logging of ksu"
    find -L "$KSU_LINK" -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/\bksu_pr_info(/pr_debug(/g' {} +
    find -L "$KSU_LINK" -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's/\bpr_info(/pr_debug(/g' {} +
else
    echo "[*] ksu is missing! Aborted"
    exit 1
fi

if [[ "$CLEAN" == true ]]; then
    echo "[*] Cleaning out directory..."
    rm -rf out
fi

if [[ "$REGEN" == true ]]; then
    echo "[*] Regenerating defconfig..."
    mkdir -p out
    make O=out ARCH=arm64 $DEFCONFIG savedefconfig
    cp out/defconfig arch/arm64/configs/$DEFCONFIG
    echo "[*] Defconfig saved to arch/arm64/configs/$DEFCONFIG"
    exit
fi

mkdir -p out
echo "[*] Generating .config..."
make O=out ARCH=arm64 $DEFCONFIG
echo -e "${bold}[*] Compiling Kernel...${normal}"
make -j$(nproc --all) O=out ARCH=arm64 \
    CC="ccache clang" LD="ccache ld.lld" LLVM=1 LLVM_IAS=1 \
    CROSS_COMPILE=aarch64-linux-gnu- \
    KCFLAGS="-fno-ident -fno-asynchronous-unwind-tables -fno-unroll-loops" \
    CROSS_COMPILE_ARM32=arm-linux-gnueabi- 2>&1 | tee log.txt

echo -e "==========================="
echo -e "   COMPILE KERNEL COMPLETE "
echo -e "==========================="

# TMP DROP IT
#cd out/arch/arm64/boot
#curl -LSs "https://raw.githubusercontent.com/ShirkNeko/SukiSU_patch/refs/heads/main/kpm/patch_linux" -o patch
#chmod 777 patch
#./patch
#mv -f oImage Image
#cd -

echo -e "${bold}[*] Proceeding to AnyKernel3 Zip Creation...${normal}"
if [ -f "$IMAGE" ]; then
    export ZIPNAME="${KERNELNAME}_Kernel_stone_${VARI}_$(date +%Y%m%d-%H%M%S).zip"
    export FINAL_ZIP="${ZIP_DIR}/${ZIPNAME}"
    rm -rf "${ZIP_DIR}"
    mkdir -p "${ZIP_DIR}"
    echo -e "[*] Injecting KERNELNAME ($KERNELNAME) to AnyKernel3..."
    sed -i "s/^kernel\.string=[^ ]*/kernel.string=${KERNELNAME}/" "${ANYKERNEL}/anykernel.sh"
    echo -e "[*] Copying kernel image to AnyKernel3"
    cp -rf -v "${IMAGE}" "${ANYKERNEL}/"
    #cp -rf -v "${DTBO}" "${ANYKERNEL}/"
    cd "${ANYKERNEL}" || exit
    echo -e "[*] Zipping kernel..."
    zip -r9 "${FINAL_ZIP}" * -x .git README.md *placeholder
    echo -e "[*] Erasing trace (Removing Image & Restoring anykernel.sh)..."
    rm -f "Image"
    #rm -f "dtbo.img"
    git restore anykernel.sh 2>/dev/null || git checkout anykernel.sh 2>/dev/null
    echo -e "[*] Uploading to GoFile..."
    curl -LSs "https://raw.githubusercontent.com/lordgaruda/GoFile-Upload/master/upload.sh" | bash -s "${FINAL_ZIP}"
    cd "${KERNELDIR}" || exit
else
    echo -e "[!] Build failed! Image not found at $IMAGE. Skipping ZIP."
    exit 1
fi

if [[ ":v" ]]; then
    exit
fi

