
source ~/bin/setALL_PATHS.sh

export VIP07U22ROOTDIR=/export/VIP/MY_GAMOS6_U22/external/root/6.26.06/root/
export VIP07U22ROOTINCDIR=${HOMEROOTDIR}/include/
export VIP07U22ROOTLIBDIR=${HOMEROOTDIR}/lib/

export VIP07U22ROOTLIBS=$(root-config --libs)
#	echo ROOT dirs: ${VIP07U22ROOTINCDIR} " " ${VIP07U22ROOTLIBDIR} 

export TEST=$( echo ${VIP07U22ROOTLIBS} | awk -v rd=${VIP07U22ROOTDIR} '{ gsub("rootbuild",rd); print $0 }' )
#	echo TEST: ${TEST}

export VIP07U22ROOTLIBS=$TEST
#	echo VIP07U22ROOTLIBS: ${VIP07U22ROOTLIBS}

BASEDIR=/home/mkolstein/VIP_PET/trunk/

export VIP_COMMON_LIB_DIR=${BASEDIR}/VIPCommonLib

source $VIP_COMMON_LIB_DIR/envir_VIP07_U22.sh

export BOOST_DIR=/usr/lib/x86_64-linux-gnu/


