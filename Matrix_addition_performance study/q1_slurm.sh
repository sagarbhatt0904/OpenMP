#!/bin/sh
#SBATCH --partition=general-compute
#SBATCH --time=00:10:00
#SBATCH --nodes=1  
#SBATCH --job-name="HPC_2_Q1"
#SBATCH --output=pi.out
#SBATCH --mail-user=sbhatt2@buffalo.edu
#SBATCH --mail-type=END
#SBATCH --requeue

echo "SLURM_JOBID="$SLURM_JOBID
echo "SLURM_JOB_NODELIST"=$SLURM_JOB_NODELIST
echo "SLURM_NNODES"=$SLURM_NNODES
echo "SLURMTMPDIR="$SLURMTMPDIR

cd $SLURM_SUBMIT_DIR
echo "working directory = "$SLURM_SUBMIT_DIR


module load intel/16.0
module load papi
module list

ulimit -s unlimited


echo "Problem 1 matrix addition: \n"
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so

/var/lib/pcp/pmdas/perfevent/perfalloc &

icpc -I$PAPI/include timer.C mat_add_row_papi.cpp -o mat_add_row_papi -L$PAPI/lib -lpapi
icpc -I$PAPI/include timer.C mat_add_col_papi.cpp -o mat_add_col_papi -L$PAPI/lib -lpapi
icpc -I$PAPI/include timer.C mat_add_row_papi_mflops.cpp -o mat_add_row_papi_mflops -L$PAPI/lib -lpapi
icpc -I$PAPI/include timer.C mat_add_col_papi_mflops.cpp -o mat_add_col_papi_mflops -L$PAPI/lib -lpapi

sh q1.sh


echo "All Jobs Completed!!!"



