#!/bin/sh

# Arguments:
# 1: src file
# 2: output prefix
# 3: output suffix

#runs ./prepare_query.py JOB_ID <SRC_FILE >(OUTPUT_PREFIX + JOB_ID + OUTPUT_SUFFIX)

./prepare_query.py $SGE_TASK_ID <$1 | ./shuffle.py >$2${SGE_TASK_ID}$3
