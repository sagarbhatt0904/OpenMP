#!/bin/sh

make

#./goldbach

#./goldbach_mflops

./goldbach_omp_s

./goldbach_omp_g

./goldbach_omp_r

./goldbach_omp_d

rm goldbach goldbach_mflops goldbach_omp_s goldbach_omp_d goldbach_omp_g goldbach_omp_r