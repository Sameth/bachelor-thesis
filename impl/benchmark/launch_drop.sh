#!/bin/sh

./drop_for_coverage.py 4700000 $SGE_TASK_ID <data/ecoli/ecoli.fasta >data/ecoli/coverages/${SGE_TASK_ID}.fasta
