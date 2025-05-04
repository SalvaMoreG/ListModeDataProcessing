
source ~/bin/setALL_PATHS_G6.sh

SOFTROOT=/home/${USER}/VIP_repository/trunk/
export VIP_COMMON_LIB_DIR=$SOFTROOT/VIPCommonLib


### IF USING GAMOS 6.1.0
### <<<<

export VIP09ROOTDIR=/export/VIP/MY_GAMOS6_VIP09/external/root/6.18.00/root/
export VIP09ROOTINCDIR=${VIP09ROOTDIR}/include/
export VIP09ROOTLIBDIR=${VIP09ROOTDIR}/lib/

export VIP09ROOTLIBS=$(root-config --libs)

export TEST=$( echo ${VIP09ROOTLIBS} | awk -v rd=${VIP09ROOTDIR} '{ gsub("rootbuild",rd); print $0 }' )

export VIP09ROOTLIBS=$TEST

### >>>

