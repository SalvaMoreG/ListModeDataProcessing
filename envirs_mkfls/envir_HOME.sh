
source ~/bin/setALL_PATHS.sh

export HOMEROOTDIR=/home/mkolstein/Programs/GAMOS/external/root/6.18.00/root/
export HOMEROOTINCDIR=${HOMEROOTDIR}/include/
export HOMEROOTLIBDIR=${HOMEROOTDIR}/lib/

export HOMEROOTLIBS=$(root-config --libs)

#	echo ROOT lib dir: ${HOMEROOTLIBDIR} 
#	echo ROOT libs: ${HOMEROOTLIBS} 
#	echo ROOT inc dir: ${HOMEROOTINCDIR} 

export PARSED=$( echo ${HOMEROOTLIBS} | awk -v rd=${HOMEROOTDIR} '{ gsub("rootbuild",rd); print $0 }' )
#	echo TEST: ${PARSED}

export HOMEROOTLIBS=$PARSED

echo HOMEROOTLIBS: ${HOMEROOTLIBS}

VIPCBASEDIR=/home/mkolstein/VIP/trunk/
export VIP_COMMON_LIB_DIR=${VIPCBASEDIR}/VIPCommonLib






