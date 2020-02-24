#!/bin/bash

PKG_NAME="qib"

QHOME=$PREFIX/q
mkdir -p $QHOME/pkg/${PKG_NAME}/app
mkdir -p $QHOME/l64

cd ${SRC_DIR}/src
mkdir build && cd build
cmake -DBUILD_x64=ON ..
make

cp ${SRC_DIR}/src/bin/*.so     $QHOME/l64
cp ${SRC_DIR}/src/init.q       $QHOME/pkg/${PKG_NAME}
cp ${SRC_DIR}/src/app/ib.q     $QHOME/pkg/${PKG_NAME}/app
cp ${SRC_DIR}/src/app/debug.q  $QHOME/pkg/${PKG_NAME}/app
cp ${SRC_DIR}/src/app/syms.csv $QHOME/pkg/${PKG_NAME}/app
