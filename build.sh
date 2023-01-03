set -x
set -e

# todo: use release build for performance in final benchmarking
bin/gn gen out/Static --args='is_official_build=false extra_cflags_cc=["-frtti"]'
ninja -C out/Static zombie
