export seastar_dir=/home/laitasso/code/seastar/seastar

cmake from build
cmake -DCMAKE_PREFIX_PATH="$seastar_dir/build/release;$seastar_dir/build/release/_cooking/installed;/home/laitasso/local/mount_build/lib/cmake; /home/laitasso/code/valgrind/" -DCMAKE_MODULE_PATH=$seastar_dir/cmake ../

make
