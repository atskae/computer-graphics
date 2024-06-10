make clean;
rm output.png;
make;
./tinyrenderer > log.txt;
echo "Logged into log.txt";
convert output.tga output.png;
code output.png;
# gprof tinyrenderer gmon.out > profile.out
