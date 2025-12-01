#!/bin/bash
# Script outline to install and build kernel.
# Author: Siddhant Jajoo.

set -e
set -u

OUTDIR=/tmp/aeld
KERNEL_REPO=git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
KERNEL_VERSION=v5.15.163
BUSYBOX_VERSION=1_33_1
FINDER_APP_DIR=$(realpath $(dirname $0))
ARCH=arm64
CROSS_COMPILE=aarch64-none-linux-gnu-

if [ $# -lt 1 ]
then
	echo "Using default directory ${OUTDIR} for output"
else
	OUTDIR=$1
	echo "Using passed directory ${OUTDIR} for output"
fi

mkdir -p ${OUTDIR}

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/linux-stable" ]; then
    #Clone only if the repository does not exist.
	echo "CLONING GIT LINUX STABLE VERSION ${KERNEL_VERSION} IN ${OUTDIR}"
	git clone ${KERNEL_REPO} --depth 1 --single-branch --branch ${KERNEL_VERSION}
fi
if [ ! -e ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ]; then
    cd linux-stable
    echo "Checking out version ${KERNEL_VERSION}"
    git checkout ${KERNEL_VERSION}



    # TODO: Add your kernel build steps here
    
    export ARCH=arm64
    
    export CROSS_COMPILE=aarch64-none-linux-gnu-
    # export CROSS_COMPILE=/home/mnunan/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-
 
     make mrproper
    
    make defconfig
    
    make -j4 all
    
    make modules
    
    make dtbs   
    
fi

echo "Adding the Image in outdir"
cp ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ${OUTDIR}/


echo "Creating the staging directory for the root filesystem"
cd "$OUTDIR"
if [ -d "${OUTDIR}/rootfs" ]
then
	echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
    sudo rm  -rf ${OUTDIR}/rootfs
    #rm  -rf ${OUTDIR}/rootfs
fi

# TODO: Create necessary base directories

mkdir rootfs
cd rootfs
mkdir -p bin dev etc home lib lib64 proc sbin sys tmp usr var
mkdir -p usr/bin usr/lib usr/sbin
mkdir -p var/log

cd "$OUTDIR"
if [ ! -d "${OUTDIR}/busybox" ]
then
#git clone git://busybox.net/busybox.git
git clone https://git.busybox.net/busybox.git
    cd busybox
    git checkout ${BUSYBOX_VERSION}
    # TODO:  Configure busybox
    #sudo sed -i 's|default "./_install"|default "/tmp/aeld/rootfs"|' Config.in
    #sudo sed -i 's|default "./_install"|default "${OUTDIR}/rootfs"|' Config.in
    #sudo sed -i 's|default "./_install"|default "'"${OUTDIR}/rootfs"'"|' Config.in

#    sudo sed -i "s|default \"./_install\"|default \"${OUTDIR}/rootfs\"|" Config.in
     sed -i "s|default \"./_install\"|default \"${OUTDIR}/rootfs\"|" Config.in
 
else
    cd busybox
    #sudo sed -i 's|default "./_install"|default "/tmp/aeld/rootfs"|' Config.in
    #sudo sed -i 's|default "./_install"|default "${OUTDIR}/rootfs"|' Config.in
    #sudo sed -i 's|default "./_install"|default "'"${OUTDIR}/rootfs"'"|' Config.in
    #sudo sed -i "s|default \"./_install\"|default \"${OUTDIR}/rootfs\"|" Config.in

#    sudo sed -i "s|default \"./_install\"|default \"${OUTDIR}/rootfs\"|" Config.in
     sed -i "s|default \"./_install\"|default \"${OUTDIR}/rootfs\"|" Config.in

fi

# TODO: Make and install busybox

    export ARCH=${ARCH}
    export CROSS_COMPILE=${CROSS_COMPILE}
    export CONFIG_PREFIX=${OUTDIR}/rootfs
    make distclean
        make defconfig
   make -j4 all
    
    cp ${OUTDIR}/busybox/busybox ${OUTDIR}/rootfs/bin/
    
    make install
    
#cp ${OUTDIR}/busybox/busybox ${OUTDIR}/rootfs/bin/

cd ${OUTDIR}/rootfs

echo "Library dependencies"
${CROSS_COMPILE}readelf -a bin/busybox | grep "program interpreter"
${CROSS_COMPILE}readelf -a bin/busybox | grep "Shared library"

    export CROSS_COMPILE=aarch64-none-linux-gnu-
    # export CROSS_COMPILE=/home/mnunan/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-

# TODO: Add library dependencies to rootfs
#cp ${CROSS_COMPILE}/libc/lib/ld-linux-aarch64.so.1 ${OUTDIR}/rootfs/lib/

#export MY_CROSS=$HOME/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu
#cp ${CROSS_COMPILE}/libc/lib/ld-linux-aarch64.so.1 ${OUTDIR}/rootfs/lib/
#export MY_CROSS=/home/mnunan/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu


localCROSSCOMPILE=`aarch64-none-linux-gnu-gcc -print-sysroot`

cp ${localCROSSCOMPILE}/lib/ld-linux-aarch64.so.1 ${OUTDIR}/rootfs/lib/

#cp ${MY_CROSS}/aarch64-none-linux-gnu/libc/lib/ld-linux-aarch64.so.1 ${OUTDIR}/rootfs/lib/

#cp ${MY_CROSS}/aarch64-none-linux-gnu/libc/lib64/libm.so.6 ${OUTDIR}/rootfs/lib64/
#cp ${MY_CROSS}/aarch64-none-linux-gnu/libc/lib64/libresolv.so.2 ${OUTDIR}/rootfs/lib64/
#cp ${MY_CROSS}/aarch64-none-linux-gnu/libc/lib64/libc.so.6 ${OUTDIR}/rootfs/lib64/

cp ${localCROSSCOMPILE}/lib64/libm.so.6 ${OUTDIR}/rootfs/lib64/
cp ${localCROSSCOMPILE}/lib64/libresolv.so.2 ${OUTDIR}/rootfs/lib64/
cp ${localCROSSCOMPILE}/lib64/libc.so.6 ${OUTDIR}/rootfs/lib64/


# TODO: Make device nodes
#777
sudo mknod -m 777 dev/null c 1 3
sudo mknod -m 777 dev/console c 5 1

# TODO: Clean and build the writer utility

cd ${FINDER_APP_DIR}

echo "building WRITER "
if [ -e "writer" ]
then
make clean
fi
make

# TODO: Copy the finder related scripts and executables to the /home directory
# on the target rootfs
cp *sh ${OUTDIR}/rootfs/home/
cp writer ${OUTDIR}/rootfs/home/
cp writer ${OUTDIR}/rootfs/usr/bin/
cp writer.c ${OUTDIR}/rootfs/home/
cp Makefile ${OUTDIR}/rootfs/home/
cp -r conf ${OUTDIR}/rootfs/home/

chmod 777 ${OUTDIR}/rootfs/home/writer


# TODO: Chown the root directory

#sudo chown -R root:root *
sudo chown -R root:root ${OUTDIR}/rootfs/
sudo chmod 777 ${OUTDIR}/rootfs

# TODO: Create initramfs.cpio.gz
cd ${OUTDIR}/rootfs/
find . | cpio -H newc -ov --owner root:root > ${OUTDIR}/initramfs.cpio
cd ..
gzip -f initramfs.cpio
