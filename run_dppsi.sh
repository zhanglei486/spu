#!/bin/sh

bazel build //... -c opt

./bazel-bin/examples/cpp/simple_dp_psi -rank 0 -protocol 1 -in_path examples/data/psi_1.csv -field_names id &
./bazel-bin/examples/cpp/simple_dp_psi -rank 1 -protocol 1 -in_path examples/data/psi_2.csv -field_names id -out_path /tmp/p2.out

