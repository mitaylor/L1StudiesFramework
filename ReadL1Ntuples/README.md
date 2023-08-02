### Summary

This is an example for how to read the emulated trigger decisions from L1Ntuples created with a Heavy Ion menu. Versions between L1Menu_CollisionsHeavyIons2018_v4_2_0 and L1Menu_CollisionsHeavyIons2023_v0_0_3 are supported. If you need a test file please contact me.

If you are using a custom menu, you can add a menu map to include/Menu.h by using the script data/makeMenuMap.sh to process your custom menu. Please contact me if you need help.

### Running instructions

1. Run cmsenv in CMSSW_13_1_0_pre4

   ```
   cd ~/L1StudiesFramework/CMSSW_13_1_0_pre4/src
   cmsenv
   ```

2. Compile

   ```
   cd ../../ReadL1Ntuples
   make
   ```

3. Run with a test L1Ntuple

   ```
   ./bin/PrintDecisions data/<L1Ntuple_file>
   ```