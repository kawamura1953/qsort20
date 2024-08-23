#!/bin/bash

cc  -O1  -w  -fcommon  main_prog.c                -o my_qsort
                                                             
cc  -O1  -w                        -c qsort20.c              
cc  -O1  -w  -fcommon  main_prog.c    qsort20.o   -o qsort20 

prin=benchmark

echo '----------------- benchmark.txt begin -------------------- '           >$prin.txt
echo ' '                                                                    >>$prin.txt
echo '�@�@�L�[:���l�Ȃ� �v�f:1���@�v�f�T�C�Y:8,20,80,1000byte '           >>$prin.txt
time ./my_qsort    -3      10000         8       4000      -1  -1  -1  0    >>$prin.txt
time ./qsort20     -3      10000         8       4000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3      10000        20       3000      -1  -1  -1  0    >>$prin.txt
time ./qsort20     -3      10000        20       3000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3      10000        80       3000      -1  -1  -1  0    >>$prin.txt
time ./qsort20     -3      10000        80       3000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3      10000      1000        600      -1  -1  -1  0    >>$prin.txt
time ./qsort20     -3      10000      1000        600      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo '�@�L�[���:100��@�v�f:1���@�v�f�T�C�Y:8,20,80,1000byte '           >>$prin.txt
time ./my_qsort   100      10000         8       6000      -1  -1  -1  0    >>$prin.txt
time ./qsort20    100      10000         8       6000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort   100      10000        20       3000      -1  -1  -1  0    >>$prin.txt
time ./qsort20    100      10000        20       3000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort   100      10000        80       4000      -1  -1  -1  0    >>$prin.txt
time ./qsort20    100      10000        80       4000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort   100      10000      1000        500      -1  -1  -1  0    >>$prin.txt
time ./qsort20    100      10000      1000        500      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo '�@�@�L�[���:2��@�v�f:1���@�v�f�T�C�Y:8,20,80,1000byte '           >>$prin.txt
time ./my_qsort     2      10000         8      30000      -1  -1  -1  0    >>$prin.txt
time ./qsort20      2      10000         8      30000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort     2      10000        20      13000      -1  -1  -1  0    >>$prin.txt
time ./qsort20      2      10000        20      13000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort     2      10000        80      12000      -1  -1  -1  0    >>$prin.txt
time ./qsort20      2      10000        80      12000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort     2      10000      1000       1200      -1  -1  -1  0    >>$prin.txt
time ./qsort20      2      10000      1000       1200      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo '�@ �L�[:���l�Ȃ��@�S�`�S���v�f�@�v�f�T�C�Y:200byte '                  >>$prin.txt
time ./my_qsort    -3        100       200     400000      -1  -1  -1  0    >>$prin.txt
time ./qsort20     -3        100       200     400000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3       1000       200      28000      -1  -1  -1  0    >>$prin.txt
time ./qsort20     -3       1000       200      28000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3      10000       200       2000      -1  -1  -1  0    >>$prin.txt
time ./qsort20     -3      10000       200       2000      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3     100000       200        120      -1  -1  -1  0    >>$prin.txt
time ./qsort20     -3     100000       200        120      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
time ./my_qsort    -3    1000000       200         11      -1  -1  -1  0    >>$prin.txt
time ./qsort20     -3    1000000       200         11      -1  -1  -1  0    >>$prin.txt
echo ' '                                                                    >>$prin.txt
echo '=================  benchmark.txt end  ==================== '          >>$prin.txt
echo 'my_qsort�F�x���`�}�[�N�e�X�g���s�̌v�Z�@��� C ���C�u������ qsort '   >>$prin.txt
echo 'qsort20 �F������J���� qsort20�� qsort '                              >>$prin.txt
echo 'd=�L�[��� (d=-3�́A���j�[�N�����Bd=k�́Arand()%k �ŃL�[���Z�o�B '    >>$prin.txt
echo 'e=�v�f�� s=�v�f�T�C�Y R�J�Ԃ��� c=��r��/R T=�����b�� '           >>$prin.txt
echo '�e�s�̍Ō�̐��l���\�[�g�P�񂠂���̏�������(10�ʕb�P��)�ł� '        >>$prin.txt
echo ' '                                                                    >>$prin.txt
