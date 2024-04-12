make clean;
make;
./tinyrenderer;
#convert output.tga output.png;
#code output.png;
#gprof tinyrenderer gmon.out > profile.out
#