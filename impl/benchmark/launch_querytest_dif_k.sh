#/bin/sh

# Arguments
# 1: reads fasta
# 2: test files prefix
# 3: test files suffix
# 4: result directory

# Runs query.bin on input files found in(SOURCE_PREFIX + JOB_ID + SOURCE_SUFFIX) and fasta in READS_FASTA
# with query length = JOB_ID
# Pushes results to (RESULT_DIRECTORY + "time" + JOB_ID + ".txt") and (RESULT_DIRECTORY + "sizes" + JOB_ID + ".txt")

./query.bin $1 `./read_length.py <$1` $SGE_TASK_ID 10000 $2${SGE_TASK_ID}$3 >${4}/time${SGE_TASK_ID}.txt 2>${4}/sizes${SGE_TASK_ID}.txt
