#### tools:

- cmake version 3.19.3
- nec compilers >= 3.2.0

Cmake NOTE: there was problem with library as it was added as hardcoded pathes in shared linking. we fixed it using mini-hack as policies and other settings did not work as intended

#### build  zlib and openssl:   

https://github.com/SX-Aurora/ve-python2
set PREFIX /opt/nec/ve
and install zlib and openssl
#### build *(patches downloaded thirdparty-libs as well)*
- export nec path for boost jam file  
 `export PATH=${PATH}:/opt/nec/ve/bin`

- update submodules to get test data
  `git submodule update --init --recursive`

- make temp directory for build
```
 export arrow_dir =  /home/???/arrow #paste where is arrow
 mkdir -p ${arrow_dir}/cpp/pbuild

```

- compile arrow, parquet and tests
 ```
 cd ${arrow_dir}/cpp/pbuild
cmake … -DCMAKE_INSTALL_PREFIX=/opt/nec/ve -D CMAKE_TOOLCHAIN_FILE=cmake/aurora.cmake -DARROW_PARQUET=ON -DARROW_BUILD_TESTS=ON 
make VERBOSE=1
#sudo make install 
```
-set directories if libs were not installed
```
export VE_LD_LIBRARY_PATH=${VE_LD_LIBRARY_PATH}:${arrow_dir}/cpp/pbuild/release
```
- run tests
```
export VE_LD_LIBRARY_PATH=${VE_LD_LIBRARY_PATH}:${arrow_dir}/cpp/pbuild/googletest_ep-prefix/lib
export ARROW_TEST_DATA=${arrow_dir}/testing/data
export PARQUET_TEST_DATA=${arrow_dir}/cpp/submodules/parquet-testing/data
cd ${arrow_dir}/cpp/pbuild
ctest unittests
```
#### usage
-  linking against **static libs**.  orders matter
```
	export lib_dir=${arrow_dir}/cpp/pbuild/release
	nc++ file.cpp ${lib_dir}/libparquet.a ${lib_dir}/libarrow.a ${lib_dir}/libarrow_bundled_dependencies.a -pthread -ldl -lrt
```
- link against **dynamic libs**
 
```nc++ file.cpp -L${lib_dir} -lparquet -larrow -pthread -ldl -lrt```

