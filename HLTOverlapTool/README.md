# OverlapTool

This tool calculates trigger overlaps (and PD overlaps) for a CMS triggering emulator. 


*note: this currently requires that you have the triggers ordered by Primary Dataset. To be remedied in the future!*

A comment about the event sizes: The event size \<\<in a given PD\>\> is calculated in two ways. First, for events that fire two (or more) different triggers, it averages the event sizes of the various triggers that fired (this is labeled "estimated average event size").  Alternatively, it simply takes the largest event size of all triggers that fired that event (this is labeled "estimated max evt size").

To run it, you'll need a text file with triggers, prescales, and PDs in it. 
see ExampleInputFile.txt

you can run it by 

```` 
./GenerateOverlapInputs.py <ExampleInputFile.txt> 

--mytag <base_output_name> 

--HltTreePath <path_to_HltTree_file> 


optional:    --rate <full_rate>  --nEvents 100  --customrootpath <path_to_root>  --SetAllEvtSizes <100>  --verbosity <0 or 1>  --debug <0 or 1>

```` 

'--mytag' is simply to give the output a unique name.  ie. the output would be called Overlaps_base_output_name.root

'--HltTreePath' is the path to your root file containing the HltTree. The tree should be in a TDirectory called 'hltbitanalysis'

'--rate' is your desired raw input collision rate by which the HltTree results get scaled. 

'--nEvents' is the number of HltTree events you want to run over. 

'--customrootpath' point to your preferred root installation

'--SetAllEvtSizes' integer that will be used for the average event size of all paths (and you don't need to supply a last column in the input file).

'--debug', if this is *not* set to 0 then the .C(.h) MakeClass files will be generated but *not* run (you can do it by hand).

Please report bugs/comments to jason.kamin@cern.ch 
