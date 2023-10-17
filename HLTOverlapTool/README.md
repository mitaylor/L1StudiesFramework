# Instructions to run the HLTOverlapTool

This tool calculates trigger overlaps (and PD overlaps) for a CMS triggering emulator. Note: this requires that you have the triggers ordered by Primary Dataset.

A comment about the event sizes: The event size in a given PD is calculated in two ways. First, for events that fire two or more different triggers, it averages the event sizes of the various triggers that fired (this is the "estimated average event size").  Alternatively, it simply takes the largest event size of all triggers that fired that event (this is the "estimated max evt size").

To run the overlap tool, you'll need a text file with triggers, prescales, and PDs in it. See ExampleInputFile.txt.

###Instructions

```` 
./GenerateOverlapInputs.py ExampleInputFile.txt> --mytag <base_output_name> --HltTreePath <path_to_HltTree_file> 

optional: --rate <full_rate> --nEvents 100 --customrootpath <path_to_root> --SetAllEvtSizes <100> --verbosity <0 or 1>  --debug <0 or 1>

````

'--mytag' = give the output a unique name

'--HltTreePath' = path to your root file containing the HltTree, which should be in a TDirectory called 'hltbitanalysis'

'--rate' = desired raw input collision rate by which the HltTree results get scaled

'--nEvents' = the number of HltTree events to run over 

'--customrootpath' = point to your preferred root installation

'--SetAllEvtSizes' = integer that will be used for the average event size of all paths (and then you don't need to supply a last column in the input file)

'--debug' = if this is *not* set to 0 then the .C(.h) MakeClass files will be generated but *not* run

Please report bugs/comments to jason.kamin@cern.ch 
