#!/bin/sh

make

echo "gprof Matrix addition looping over column:"
echo "\n"

./mat_add_col

gprof mat_add_col gmon.out>col.txt

rm mat_add_col gmon.out

echo "gprof Matrix addition looping over row:"
echo "\n"

./mat_add_row

gprof mat_add_row gmon.out>row.txt

rm mat_add_row gmon.out

echo "PAPI study looping over row"
echo "\n"

./mat_add_row_papi

rm mat_add_row_papi

echo "PAPI study looping over col"
echo "\n"

./mat_add_col_papi

rm mat_add_col_papi

echo "PAPI study looping over row"
echo "\n"

./mat_add_row_papi_mflops

rm mat_add_row_papi_mflops

echo "PAPI study looping over col"
echo "\n"

./mat_add_col_papi_mflops

rm mat_add_col_papi_mflops