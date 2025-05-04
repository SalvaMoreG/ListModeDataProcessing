 
source ~/bin/setALL_PATHS_G6.sh

export VIP04ROOTDIR=/export/VIP/MY_GAMOS6_VIP09/external/root/6.18.00/root/
export VIP04ROOTINCDIR=${VIP04ROOTDIR}/include/
export VIP04ROOTLIBDIR=${VIP04ROOTDIR}/lib/

export VIP04ROOTLIBS=$(root-config --libs)
echo ROOT dirs: ${VIP04ROOTINCDIR} " " ${VIP04ROOTLIBDIR} 

export TEST=$( echo ${VIP04ROOTLIBS} | awk -v rd=${VIP04ROOTDIR} '{ gsub("rootbuild",rd); print $0 }' )
#	echo TEST: ${TEST}

export VIP04ROOTLIBS=$TEST

echo VIP04ROOTLIBS: ${VIP04ROOTLIBS}

export VIP_COMMON_LIB_DIR=/home/${USER}/VIP_PET/trunk/VIPCommonLib



