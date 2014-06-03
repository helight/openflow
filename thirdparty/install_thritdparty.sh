#!/bin/bash
# Installation of third-parties
#
# # If necessary, you can edit the following
THIRD_PARTY_HOME=`pwd`
# # INSTALL_ALL为非空的时候，表示全新安装
INSTALL_ALL=$1
#
printf "\033[1;33m"
echo "Hi:"
echo " the third-parties will be installed to $THIRD_PARTY_HOME,"
echo " you can edit THIRD_PARTY_HOME in install-thirdparty.sh to change a directory."
echo " THIRD_PARTY_HOME=$THIRD_PARTY_HOME"
printf "\033[m"
sleep 2
echo "begin to compile & install now ..."
sleep 1
# goto src-package sub-directory to compile and install
cd $THIRD_PARTY_HOME

# install function for common tar.gz
#
# install_tar_gz_package install package_name config_args
#

function install_tar_gz_package()
{
    install_dir=$1
    package_name=$2
    config_args=$3
    # 如果目录已经存在，而且不是全新安装
    if [ -d $THIRD_PARTY_HOME/$install_dir -a "$INSTALL_ALL" = "" ];then
        printf "\n\033[1;33m $install_dir is installed, no need to install again\033[m\n"
        return
    fi
    printf "\n\033[1;33m installing $package_name \033[m\n"
    tar_opt=${package_name##*.tar.}
    if [ $tar_opt = 'gz' ]; then
        tar xzf $package_name
        package_basename=`basename $package_name .tar.gz`
    elif [ $tar_opt = 'bz2' ]; then
        package_basename=`basename $package_name .tar.bz2`
        tar xjf $package_name
    else
        printf "\n\033[1;33m can't get the type of package \033[m\n"
            exit 1
    fi
    printf "\n\033[1;33m installing $package_basename \033[m\n"
    cd $package_basename
    ./configure --prefix=$THIRD_PARTY_HOME/$install_dir $config_args
    if test $? -ne 0; then
        exit 1
    fi
    make
    if test $? -ne 0; then
        exit 1
    fi
    make install
    cd -
}

function install_gtest()
{
# 如果目录已经存在，而且不是全新安装
    if [ -d $THIRD_PARTY_HOME/gtest -a "$INSTALL_ALL" = "" ];then
        printf "\n\033[1;33m gtest is installed, no need to install again\033[m\n"
        return
    fi
#
    printf "\n\033[1;33minstalling gtest \033[m\n"
#
    unzip -x gtest-1.7.0.zip
    cd gtest-1.7.0
    ./configure --prefix=$THIRD_PARTY_HOME/gtest
    make
    if test $? -ne 0; then
        exit 1
    fi
    mkdir $THIRD_PARTY_HOME/gtest
    cp include $THIRD_PARTY_HOME/gtest/ -rf
    g++ -I ./include/ -I ./ -c src/gtest-all.cc
    ar -rv libgtest.a gtest-all.o
    mkdir $THIRD_PARTY_HOME/gtest/lib/
    cp libgtest.a $THIRD_PARTY_HOME/gtest/lib/
    cd -
}

function install_all()
{
    echo "Begin to install......"
    install_tar_gz_package thrift thrift-0.9x.tar.gz
    install_tar_gz_package libev libev-4.15.tar.gz
    install_tar_gz_package gflags gflags-2.0.tar.gz
    install_tar_gz_package glog glog-0.3.3.tar.gz
    install_gtest
}

install_all

printf "\n\033[1;33minstall SUCCESS\033[m\n"

cd $THIRD_PARTY_HOME
