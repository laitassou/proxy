export seastar_dir=$ENV{HOME}/code/seastar/seastar

cmake from build
cmake -DCMAKE_PREFIX_PATH="$seastar_dir/build/release;$seastar_dir/build/release/_cooking/installed;$ENV{HOME}/local/mount_build/lib/cmake; $ENV{HOME}/code/valgrind/" -DCMAKE_MODULE_PATH=$seastar_dir/cmake ../

make
