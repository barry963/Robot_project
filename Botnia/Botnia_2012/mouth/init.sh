export LD_LIBRARY_PATH=/usr/local/java/jre1.7.0_09/lib/i386:/usr/local/java/jre1.7.0_09/lib/i386/client:$LD_LIBRARY_PATH
g++ recognitionTest_modified.cpp -o recognition_modified -I/usr/local/java/jdk1.7.0_09/include/ -I/usr/local/java/jdk1.7.0_09/include/linux -L/usr/local/java/jre1.7.0_09/lib/i386/client/ -ljvm
g++ synthesisTest_modified.cpp -o synthesis_modified -I/usr/local/java/jdk1.7.0_09/include/ -I/usr/local/java/jdk1.7.0_09/include/linux -L/usr/local/java/jre1.7.0_09/lib/i386/client/ -ljvm
