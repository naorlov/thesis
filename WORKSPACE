  
workspace(name = "bazel_latex")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "bazel_latex",
    strip_prefix = "bazel-latex-0.19",
    url = "https://github.com/ProdriveTechnologies/bazel-latex/archive/v0.19.tar.gz",
)

load("@bazel_latex//:repositories.bzl", "latex_repositories")

latex_repositories()
