#!/bin/sh

date;

#rm -f summary.txt;

secs_between_reports=0.1;
secs_of_training=5
number_of_runs=1;
decay=linear;

function benchmark_algorithm() {
    echo "./quality_gng datasets/$prob.train datasets/$prob.test $prob.train.out $secs_of_training $secs_between_reports $number_of_runs"
    date;
    ./quality_gng datasets/$prob.train datasets/$prob.test $prob.train.out $secs_of_training $secs_between_reports $number_of_runs
}

function benchmark_problem() {
    benchmark_algorithm;
}

prob="abelone";
benchmark_problem;

prob="bank32fm";
benchmark_problem;

prob="bank32nh";
benchmark_problem;

prob="kin32fm";
benchmark_problem;

prob="building";
benchmark_problem;

prob="diabetes";
benchmark_problem;

prob="pumadyn-32fm";
benchmark_problem;

prob="robot";
benchmark_problem;

prob="soybean";
benchmark_problem;

prob="thyroid";
benchmark_problem;

prob="two-spiral";
benchmark_problem;

./gnuplot_gng.pl | gnuplot
