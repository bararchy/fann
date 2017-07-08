#!/bin/sh

date;

#rm -f summary.txt;

max_training=1000;
secs_between_reports=0.1;
number_of_runs=1;
decay=linear;

function benchmark_algorithm() {
    echo "./quality_som datasets/$prob.train datasets/$prob.test $prob.$topo.$neigh.train.out $width $height $topo $neigh $decay $max_training $secs_between_reports $number_of_runs"
    date;
    ./quality_som datasets/$prob.train datasets/$prob.test $prob.$topo.$neigh.train.out $width $height $topo $neigh $decay $max_training $secs_between_reports $number_of_runs
}

function benchmark_problem() {
    topo="hexagonal"; neigh="distance";  benchmark_algorithm;
    topo="hexagonal"; neigh="gaussian";  benchmark_algorithm;
    topo="rectangular"; neigh="distance";  benchmark_algorithm;
    topo="rectangular"; neigh="gaussian";  benchmark_algorithm;
}

prob="abelone"; height=15; width=15;
benchmark_problem;

prob="bank32fm"; height=20; width=20;
benchmark_problem;

prob="bank32nh"; height=20; width=20;
benchmark_problem;

prob="kin32fm"; height=20; width=20;
benchmark_problem;

prob="building"; height=15; width=15;
benchmark_problem;

prob="diabetes"; height=10; width=10;
benchmark_problem;

prob="pumadyn-32fm"; height=30; width=30;
benchmark_problem;

prob="robot"; height=30; width=30;
benchmark_problem;

prob="soybean"; height=40; width=40;
#benchmark_problem;

prob="thyroid"; height=20; width=20;
benchmark_problem;

prob="two-spiral"; height=10; width=10;
benchmark_problem;

./gnuplot_som.pl | gnuplot
