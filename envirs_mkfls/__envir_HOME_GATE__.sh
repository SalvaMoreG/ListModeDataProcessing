
export HOMEROOTDIR=/home/mkolstein/Programs/GATE/AGAIN_GATE_9.2/external/root_from_source_v6.26.10/root_install/
export HOMEROOTINCDIR=${HOMEROOTDIR}/include/
export HOMEROOTLIBDIR=${HOMEROOTDIR}/lib/

export HOMEROOTLIBS=$(root-config --libs)
#	echo ROOT dirs: ${HOMEROOTINCDIR} " " ${HOMEROOTLIBDIR} 

export TEST=$( echo ${HOMEROOTLIBS} | awk -v rd=${HOMEROOTDIR} '{ gsub("rootbuild",rd); print $0 }' )
#	echo TEST: ${TEST}

export HOMEROOTLIBS=$TEST
#	echo HOMEROOTLIBS: ${HOMEROOTLIBS}

BASEDIR=/home/mkolstein/VIP/trunk/

export VIP_COMMON_LIB_DIR=${BASEDIR}/VIPCommonLib

source $VIP_COMMON_LIB_DIR/envir_HOME.sh

export BOOST_DIR=/usr/lib/x86_64-linux-gnu/


