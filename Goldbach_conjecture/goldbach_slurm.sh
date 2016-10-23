#!/bin/sh
#SBATCH --partition=general-compute
#SBATCH --time=01:00:00
#SBATCH --nodes=1 --ntasks-per-node=12 --constraint=CPU-E5645
#SBATCH --job-name="HPC_2_Q3"
#SBATCH --output=goldbach.out
#SBATCH --mail-user=sbhatt2@buffalo.edu
#SBATCH --mail-type=ALL
#SBATCH --requeue

echo "SLURM_JOBID="$SLURM_JOBID
echo "SLURM_JOB_NODELIST"=$SLURM_JOB_NODELIST
echo "SLURM_NNODES"=$SLURM_NNODES
echo "SLURMTMPDIR="$SLURMTMPDIR

cd $SLURM_SUBMIT_DIR
echo "working directory = "$SLURM_SUBMIT_DIR


module load gcc
module list
ulimit -s unlimited


echo -e "Goldbach Conjecture: \n"
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so
export OMP_NUM_THREADS=$SLURM_CPUS_ON_NODE


sh run.sh

echo "All Jobs Completed!!!"



