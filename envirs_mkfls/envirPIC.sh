
source /data/vip/software/GAMOS6/GAMOS.6.1.0-new/config/confgamos.sh

export VIP_COMMON_LIB_DIR=/data/vip/software/WD_VIP_repository/VipGamosCode/trunk/VIPCommonLib

export PICROOTDIR=/data/vip/software/GAMOS6/external/root/6.18.00/root
export PICROOTINCDIR=${PICROOTDIR}/include/
export PICROOTLIBDIR=${PICROOTDIR}/lib/

export PICROOTLIBS=$(root-config --libs)
echo ROOT dirs: ${PICROOTINCDIR} " " ${PICROOTLIBDIR} 

export TEST=$( echo ${PICROOTLIBS} | awk -v rd=${PICROOTDIR} '{ gsub("rootbuild",rd); print $0 }' )
echo TEST: ${TEST}

export PICROOTLIBS=$TEST

export ListModeDPPATH=/data/vip/software/WD_VIP_repository/VipGamosCode/trunk/ListModeDataProcessing/bin

export PATH=$ListModeDPPATH:$PATH

