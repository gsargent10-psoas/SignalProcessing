# SignalProcessing
The src directory contains three folders with libraries: General, Polarimetric, and Piwrapper. The General directory provides signal processing functions for general signal processing applications. The Polarimetric directory contains polarimetric imaging specific signal processing techniques. The Piwrapper directory contains the functionality to read/write polarimetric data to file. The Piwrapper folder is dependent on an Applied Sensing Lab (ASL) repository. The latest cpp and h files should be copied into the Piwrapper directory before build. There exits a "make copy" command within the Piwrapper Makefile that will pull from the repository if it exists along side the SignalProcessing directory.

# Building
From the root directory run build.sh. This script will call all nested build.sh.
```
cd SignalProcessing/
./build.sh
```