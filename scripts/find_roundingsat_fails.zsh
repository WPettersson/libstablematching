#!/usr/bin/env zsh

set -e

function do_test () {
  # SIZE PREF_LENGTH DENS INSTANCE_NUMBER
  RESULTS=$(./pbo $1 $2 $3 testing $4 | awk '/unmerged took/ {utime = $3} /merged took/ {mtime = $3} /unmerged found/ {usize = $3} /merged found/ {msize = $3} END {print utime,usize,mtime,msize}')
  RESULTS=$(../../../roundingsat/roundingsat-O3 --verbosity=0 testing-v2.pbo | awk '/CPU time/ {time = $5} /objective function value/ {size = $5} END {print time,size}')
  usat_arr=(${(s/ /)RESULTS})
  (( usat_size = $1 - $usat_arr[2] / 2 ))
  RESULTS=$(../../../roundingsat/roundingsat-O3 --verbosity=0 testing-v3.pbo | awk '/CPU time/ {time = $5} /objective function value/ {size = $5} END {print time,size}')
  msat_arr=(${(s/ /)RESULTS})
  if [ ${#msat_arr[@]} -ne 2 ] ; then
    (cd /home/enigma/src/npSolver/ ;
    ./npSolver-pbo /home/enigma/src/git/smti-encodings/build/src/testing-v3.pbo tempfile &> /dev/null || echo "merged failed on $1 $2 $3 $4"
    )
    exit
  fi
}

c=0
while true ; do
  do_test 5 2 0.85
  echo -ne "$c\r"
  (( c = $c + 1 ))
done
