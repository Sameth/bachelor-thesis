#!/bin/sh

./drop_for_coverage_fastq.py 4700000 $SGE_TASK_ID <data/ecoli/ecoli.fastq >data/ecoli/coverages/${SGE_TASK_ID}.fastq
