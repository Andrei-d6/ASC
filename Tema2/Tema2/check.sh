module load compilers/gnu-5.4.0;
make clean
make
./tema2_neopt input &> neopt.txt;
./tema2_opt_m input &> opt_m.txt;
./tema2_opt_f input &> opt_f.txt;
./tema2_opt_f_extra input &> opt_f_extra.txt;
./tema2_blas input &> blas.txt
