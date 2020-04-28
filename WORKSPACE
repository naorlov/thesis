new_local_repository(
    name = "simgrid_lib",
    path = "/opt/simgrid/lib",
    build_file_content = """
cc_library(
    name = "libsimgrid",
    srcs = ["libsimgrid.so"],
    visibility = ["//visibility:public"],
    linkopts = ["-Wl,-rpath,/opt/simgrid/lib",]
)
"""
)
new_local_repository(
    name = "simgrid_headers",
    path = "/opt/simgrid/include",
    build_file_content = """
cc_library(
    name = "simgrid",
    hdrs = glob(["**/*.h", "**/*.hpp"]),
    visibility = ["//visibility:public"],
    includes = ["."]
)  
"""
)

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "9f9fb8b2f0213989247c9d5c0e814a8451d18d7f",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1570056263 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

git_repository(
    name = "yaml-cpp",
    remote = "https://github.com/jbeder/yaml-cpp.git",
    commit = "7083db28b3d339fdd48af2b52dd8d5f0eecde98c",
    shallow_since = "1587937982 -0500"
)
