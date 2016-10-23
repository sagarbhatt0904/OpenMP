#!/bin/sh

echo ' Running Mandlebrotset Problem... '

make

#./mand_set_papi  

#./mand_omp

#./mand_omp_d

#./mand_omp_g

./mand_omp_r

echo ' Completed! '

#rm mand_set_papi mand_omp mand_omp_d mand_omp_g mand_omp_r
rm mand_omp_r
