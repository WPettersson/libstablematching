#!/usr/bin/env zsh

function insert () {
  # SIZE PREF_LENGTH DENS INSTANCE_NUMBER CPLEX_TIME CPLEX_SIZE
  # ROUNDINGSAT_TIME ROUNDINGSAT_SIZE
  sqlite3 --batch timing.sqlite3 "insert into timing values('$1', '$2', '$3', '$4', '$5', '$6', '$7', '$8', '$9', '$10', '$11', '$12');"
}


function do_test () {
  # SIZE PREF_LENGTH DENS INSTANCE_NUMBER
  RESULTS=$(./pbo $1 $2 $3 testing $4 | awk '/unmerged took/ {utime = $3} /merged took/ {mtime = $3} /unmerged found/ {usize = $3} /merged found/ {msize = $3} END {print utime,usize,mtime,msize}')
  cplex_arr=(${(s/ /)RESULTS})
  RESULTS=$(../../../roundingsat/roundingsat-O3 --verbosity=0 testing-v2.pbo | awk '/CPU time/ {time = $5} /objective function value/ {size = $5} END {print time,size}')
  usat_arr=(${(s/ /)RESULTS})
  if [ ${#usat_arr[@]} -ne 2 ] ; then
    echo "unmerged failed on $1 $2 $3 $4"
    return
  fi
  (( usat_size = $1 - $usat_arr[2] / 2 ))
  RESULTS=$(../../../roundingsat/roundingsat-O3 --verbosity=0 testing-v3.pbo | awk '/CPU time/ {time = $5} /objective function value/ {size = $5} END {print time,size}')
  msat_arr=(${(s/ /)RESULTS})
  if [ ${#msat_arr[@]} -ne 2 ] ; then
    (cd /home/enigma/src/npSolver/ ;
    ./npSolver-pbo /home/enigma/src/git/smti-encodings/build/src/testing-v3.pbo tempfile &> /dev/null || echo "merged failed on $1 $2 $3 $4"
    )
    return
  fi
  (( msat_size = $1 - $msat_arr[2] / 2 ))
  insert $1 $2 $3 $4 $cplex_arr[1] $cplex_arr[2] $cplex_arr[3] $cplex_arr[4] $usat_arr[1] $usat_size $msat_arr[1] $msat_size
}


if [ ! -e "timing.sqlite3" ] ; then
  sqlite3 timing.sqlite3 'create table timing(size INTEGER, pref_length INTEGER, dens REAL, number INTEGER, 
  cplex_utime REAL, cplex_usize INTEGER, cplex_mtime REAL, cplex_msize INTEGER,
  sat_utime REAL, sat_usize INTEGER, sat_mtime REAL, sat_msize INTEGER);';
fi

for size in {50..1000..50} ; do
  echo "On size ${size}"
  for pref_length in 3 5 10 25 ; do
    for dens in 0.75 0.85 0.95 ; do
      for iteration in {0..50} ; do
        do_test $size $pref_length $dens $iteration
      done
    done
  done
done
