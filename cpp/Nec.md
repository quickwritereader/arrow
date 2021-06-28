#### tools:
```
$ cmake --version
cmake version 3.19.3

CMake suite maintained and supported by Kitware (kitware.com/cmake).
$ nc++ --version
nc++ (NCC) 3.2.1 (Build 18:49:44 Apr 23 2021)
Copyright (C) 2018,2021 NEC Corporation.
```
#### zlib and openssl:   

https://github.com/SX-Aurora/ve-python2
set PREFIX /opt/nec/ve
and install zlib and openssl
#### compile (patches downloaded thrift):
```
mkdir -p arrow/cpp/pbuild
cd arrow/cpp/pbuild
#just compile
cmake  .. -D CMAKE_TOOLCHAIN_FILE=cmake/aurora.cmake -DARROW_PARQUET=ON


# with tests linked against staticlibs besides gtest* gmock*.
cmake  .. -DCMAKE_INSTALL_PREFIX=/opt/nec/ve  -D CMAKE_TOOLCHAIN_FILE=cmake/aurora.cmake -DARROW_PARQUET=ON -DARROW_BUILD_TESTS=ON -DARROW_LINK_SHARED=OFF -DARROW_DEPENDENCY_USE_SHARED=OFF -DARROW_TEST_LINKAGE=static
make VERBOSE=1

# as we noted tests still needs gtest libs:
# assuming you set root arrow directory set arrow_dir=  #${arrow_dir}
export VE_LD_LIBRARY_PATH=${VE_LD_LIBRARY_PATH}:${arrow_dir}/cpp/pbuild/googletest_ep-prefix/lib

#update submodules if you did not
git submodule update --init --recursive

you can manually set and run individual tests or just use ready scripts in ci/scripts

export ARROW_TEST_DATA=${arrow_dir}/testing/data
export PARQUET_TEST_DATA=${arrow_dir}/cpp/submodules/parquet-testing/data
```
