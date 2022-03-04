### Summary

This is an example for how to read the emulated trigger decisions from L1Ntuples created with the 2018 PbPb menu (L1Menu_CollisionsHeavyIons2018_v4_2_0.xml) or the updated baseline Run 3 menu (L1Menu_CollisionsHeavyIons2022_v0_0_0.xml). If you need a test file please contact me.

If you are using a custom menu, you can edit the menu map in src/PrintDecisions.C by using the script data/makeMenuMap.sh to process your custom menu. Please contact me if you need help.

### Running instructions

1. Run cmsenv in CMSSW_12_3_0_pre1

   ```
   cd ~/L1StudiesFramework/CMSSW_12_3_0_pre1/src
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