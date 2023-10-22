### Instructions for makeMenuMap.sh

To process a L1Ntuples produced with a custom menu, you need to update the menu map in ../src/PrintDecisions.C. The script makeMenuMap.sh reads the menu XML file and creates a map of the trigger bit to trigger name based on its contents, which you can then add to ../include/Menu.h.

To run, do:

```
./makeMenuMap.sh <menu_name>
```

