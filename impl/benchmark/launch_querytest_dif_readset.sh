#!/bin/sh

./query.bin data/ecoli/coverages/${SGE_TASK_ID}.fasta 150 20 10000 data/ecoli/ecoli20.in >data/results/sr/ecoli/coverages/time_${SGE_TASK_ID}.txt 2>data/results/sr/ecoli/coverages/sizes_${SGE_TASK_ID}.txt
