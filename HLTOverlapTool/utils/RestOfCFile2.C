

    Int_t AlreadyFired_L1_raw = 0;
    Int_t AlreadyFiredHlt_raw = 0;
    Int_t AlreadyFired_L1_psc = 0;
    Int_t AlreadyFiredHlt_psc = 0;

    for(int i=0; i<nPrimaryDSet; i++){
      FiredPD_raw[i] = 0;
      FiredPD_psc[i] = 0;
    }
    for(int i=0; i<nTriggers_L1; i++){
      FiredL1_raw[i] = 0;
      FiredL1_psc[i] = 0;
    }
    for(int i=0; i<nTriggersHlt; i++){
      FiredTr_raw[i] = 0;
      FiredTr_psc[i] = 0;
    }

    Double_t thisEvtSizeAve_psc= 0.0;
    Double_t thisEvtSizeMax_psc = 0.0;
    Double_t thisEvtSizeAve_raw = 0.0;
    Double_t thisEvtSizeMax_raw = 0.0;


    for(int i=0; i<nTriggers_L1; i++){//--------------------------------------------------------------------------------------- 
      if(My_L1[i]){//---------------------------------------------------------------------------------------------------    //| 
        nEvts_L1_raw[i]++;                                                                                           //|    //|L
        FiredL1_raw[i] = 1;                                                                                          //|I   //|O
        h_freq_L1_raw[i]->Fill(jentry-last_jentry_L1_raw[i]);                                                        //|F   //|O
        last_jentry_L1_raw[i] = jentry;                                                                              //|    //|P
        if(AlreadyFired_L1_raw==0)                                                                                   //|I   //| 
          TotalEvt_L1_raw++;                                                                                         //|F   //|L
        AlreadyFired_L1_raw++;                                                                                       //|    //|O
                                                                                                                     //|I   //|O
        for(int j=0; j<nTriggersHlt; j++)                                                                            //|F   //|P
          if(MyHLT[j] && L1PathForHLT[j]==i){                                                                        //|    //| 
            nEvtsHlt_ps1[j]++;                                                                                       //|I   //|L
            FiredTr_raw[j] = 1;                                                                                      //|F   //|O
            FiredPD_raw[PDForHltID[j]] = 1;                                                                          //|    //|O
            h_freq_HLT_raw[j]->Fill(jentry-last_jentry_HLT_raw[j]);                                                  //|I   //|P
            last_jentry_HLT_raw[j] = jentry;                                                                         //|F   //|
            if(EvtSizeHlt[j]>thisEvtSizeMax_raw)                                                                     //|    //|L
              thisEvtSizeMax_raw = EvtSizeHlt[j];                                                                    //|I   //|O
            thisEvtSizeAve_raw  += EvtSizeHlt[j];                                                                    //|F   //|O
          }                                                                                                          //|    //|P
                                                                                                                     //|I   //| 
        if(Int_t(nEvts_L1_raw[i])%Int_t(PreScale_L1[i])==0){ //--------------------------------------------------    //|F   //|L
          nEvts_L1_psc[i]++;                                                                                  //|I   //|    //|O
          FiredL1_psc[i] = 1;                                                                                 //|F   //|I   //|O
          h_freq_L1_psc[i]->Fill(jentry-last_jentry_L1_psc[i]);                                               //|    //|F   //|P
          last_jentry_L1_psc[i] = jentry;                                                                     //|    //|    //|
          if(AlreadyFired_L1_psc==0)                                                                          //|    //|I   //|L
            TotalEvt_L1_psc++;                                                                                //|I   //|F   //|O
          AlreadyFired_L1_psc++;                                                                              //|F   //|    //|O
                                                                                                              //|    //|I   //|P
          for(int j=0; j<nTriggersHlt; j++){ //-----------------------------------------------------          //|I   //|F   //| 
            if(MyHLT[j] && L1PathForHLT[j]==i){ //-------------------------------------------    //|          //|F   //|    //|L
              nEvtsHlt_raw[j]++;                                                          //|    //|L         //|    //|I   //|O
              h_freq_HLT_psc[j]->Fill(jentry-last_jentry_HLT_psc[j]);                     //|I   //|O         //|I   //|F   //|O
              last_jentry_HLT_psc[j] = jentry;                                            //|F   //|O         //|F   //|    //|P
              if(AlreadyFiredHlt_raw==0)                                                  //|    //|P         //|I   //|I   //| 
                TotalEvtHlt_raw++;                                                        //|I   //|          //|F   //|F   //|L
              AlreadyFiredHlt_raw++;                                                      //|F   //|L         //|    //|    //|O
                                                                                          //|    //|O         //|I   //|I   //|O
              //if(Int_t(nEvtsHlt_raw[j])%Int_t(PreScaleHlt[j])==0){ //---------------    //|    //|O         //|F   //|I   //|O
              if(Int_t(nEvts_L1_psc[L1PathForHLT[j]])%Int_t(PreScaleHlt[j])==0){ //---    //|I   //|O         //|F   //|F   //|P
                nEvtsHlt_psc[j]++;                                                 //|I   //|F   //|P         //|    //|    //| 
                FiredTr_psc[j] = 1;                                                //|F   //|    //|          //|I   //|I   //|L
                FiredPD_psc[PDForHltID[j]] = 1;                                    //|    //|I   //|L         //|F   //|F   //|O
                if(EvtSizeHlt[j]>thisEvtSizeMax_psc)                               //|I   //|F   //|O         //|    //|    //|O
                  thisEvtSizeMax_psc = EvtSizeHlt[j];                              //|F   //|    //|O         //|I   //|I   //|P
                thisEvtSizeAve_psc  += EvtSizeHlt[j];                              //|    //|I   //|P         //|F   //|F   //| 
                if(AlreadyFiredHlt_psc==0)                                         //|I   //|F   //|          //|    //|    //|L
                  TotalEvtHlt_psc++;                                               //|F   //|    //|L         //|I   //|I   //|O
                AlreadyFiredHlt_psc++;                                             //|    //|I   //|O         //|F   //|F   //|O
              }// HLT pScale //-------------------------------------------------------    //|F   //|O         //|    //|    //|P
                                                                                          //|    //|P         //|I   //|I   //| 
            }// if HLT fired //--------------------------------------------------------------    //|          //|F   //|F   //|L
          }// HLT loop //---------------------------------------------------------------------------          //|    //|    //|O
                                                                                                              //|    //|I   //|O
        }// L1 pScale //-----------------------------------------------------------------------------------------    //|F   //|P
                                                                                                                     //|    //| 
      }// if L1 fired //------------------------------------------------------------------------------------------------    //| 
    }// L1 loop //-------------------------------------------------------------------------------------------------------------


    for(int i=0; i<nTriggers_L1; i++){
      for(int j=0; j<nTriggers_L1; j++){
        if(FiredL1_raw[i]==1 && FiredL1_raw[j]==1)
          h_L1_L1_raw->Fill(i+1,j+1);
        if(FiredL1_psc[i]==1 && FiredL1_psc[j]==1)
          h_L1_L1_psc->Fill(i+1,j+1);
      }
    }

    for(int i=0; i<nTriggersHlt; i++){
      for(int j=0; j<nTriggersHlt; j++){
        if(FiredTr_raw[i]==1 && FiredTr_raw[j]==1)
          h_tr_tr_raw->Fill(i+1,j+1);
        if(FiredTr_psc[i]==1 && FiredTr_psc[j]==1)
          h_tr_tr_psc->Fill(i+1,j+1);
      }
    }

    for(int i=0; i<nTriggersHlt; i++){
      for(int j=0; j<nPrimaryDSet; j++){
        if(FiredTr_raw[i]==1 && FiredPD_raw[j]==1)
          h_tr_pd_raw->Fill(i+1,j+1);
        if(FiredTr_psc[i]==1 && FiredPD_psc[j]==1)
          h_tr_pd_psc->Fill(i+1,j+1);
      }
    }

    for(int i=0; i<nPrimaryDSet; i++){
      for(int j=0; j<nPrimaryDSet; j++){
        if(FiredPD_raw[i]==1 && FiredPD_raw[j]==1)
          h_pd_pd_raw->Fill(i+1,j+1);
        if(FiredPD_psc[i]==1 && FiredPD_psc[j]==1)
          h_pd_pd_psc->Fill(i+1,j+1);
      }
    }

    Int_t NumTrigFired_psc = 0;
    Int_t NumTrigFired_raw = 0;
    for(int i=0; i<nTriggers_L1; i++){
      if(FiredL1_psc[i]==1)  h_RatesPerL1_psc->Fill(i+1);
      if(FiredL1_raw[i]==1)  h_RatesPerL1_raw->Fill(i+1);
    }
    for(int i=0; i<nTriggersHlt; i++){
      if(FiredTr_psc[i]==1)  h_RatesPerTr_psc->Fill(i+1);
      if(FiredTr_raw[i]==1)  h_RatesPerTr_raw->Fill(i+1);
      if(FiredTr_psc[i]==1)  NumTrigFired_psc++;
      if(FiredTr_raw[i]==1)  NumTrigFired_raw++;
    }
    for(int j=0; j<nPrimaryDSet; j++){
      if(FiredPD_psc[j]==1)  h_RatesPerPD_psc->Fill(j+1);
      if(FiredPD_raw[j]==1)  h_RatesPerPD_raw->Fill(j+1);
    }
    thisEvtSizeAve_psc = thisEvtSizeAve_psc/double(NumTrigFired_psc);
    thisEvtSizeAve_raw = thisEvtSizeAve_raw/double(NumTrigFired_raw);

    Int_t nPDsFired = 0;
    for(int i=0; i<nPrimaryDSet; i++){
      Int_t nOverlappingPDs = 0;
      if(FiredPD_psc[i]==1){
        TotSizeMaxPD_psc[i] += thisEvtSizeMax_psc;
        TotSizeAvePD_psc[i] += thisEvtSizeAve_psc;
        nPDsFired++;
        for(int j=0; j<nPrimaryDSet; j++)
          if(i!=j && FiredPD_psc[j]==1)
            nOverlappingPDs++;
        h_PDs_NumOtherPDsFired[i]->Fill(nOverlappingPDs);
        if(nOverlappingPDs==0){
          h_UniqueEvts->Fill(i+1);
          h_UniqueEffs->Fill(i+1);
        }
      }//FiredPD_psc==1
      if(FiredPD_raw[i]==1){
        TotSizeMaxPD_raw[i] += thisEvtSizeMax_raw;
        TotSizeAvePD_raw[i] += thisEvtSizeAve_raw;
      }//FiredPD_raw==1
    }//loop over PDs
    h_nPDsFired->Fill(nPDsFired);

    evtCount++;
  }//evt loop

  for(int i=0; i<nTriggers_L1; i++){
    SumOfEvt_L1_raw += nEvts_L1_raw[i];
    SumOfEvt_L1_psc += nEvts_L1_psc[i];
  }
  for(int i=0; i<nTriggersHlt; i++){
    SumOfEvtHlt_raw += nEvtsHlt_raw[i];
    SumOfEvtHlt_psc += nEvtsHlt_psc[i];
  }

  h_UniqueEffs->Divide(h_RatesPerPD_psc);
  for(int i=0; i<nPrimaryDSet; i++)  h_TotalEvts_psc ->SetBinContent(i+1,h_RatesPerPD_psc->GetBinContent(i+1));
  for(int i=0; i<nPrimaryDSet; i++)  h_TotalEvts_raw ->SetBinContent(i+1,h_RatesPerPD_raw->GetBinContent(i+1));
  h_RatesPerPD_psc->Scale(FullRate/Double_t(evtCount));
  h_RatesPerPD_raw->Scale(FullRate/Double_t(evtCount));
  h_RatesPerL1_psc->Scale(FullRate/Double_t(evtCount));
  h_RatesPerL1_raw->Scale(FullRate/Double_t(evtCount));
  h_RatesPerTr_psc->Scale(FullRate/Double_t(evtCount));
  h_RatesPerTr_raw->Scale(FullRate/Double_t(evtCount));

  for(int i=0; i<nPrimaryDSet; i++){
    if(h_TotalEvts_psc->GetBinContent(i+1)>0){
      EvtSizeMaxPD_psc[i] = TotSizeMaxPD_psc[i]/h_TotalEvts_psc->GetBinContent(i+1);
      EvtSizeAvePD_psc[i] = TotSizeAvePD_psc[i]/h_TotalEvts_psc->GetBinContent(i+1);
    }
    if(h_TotalEvts_raw->GetBinContent(i+1)>0){
      EvtSizeMaxPD_raw[i] = TotSizeMaxPD_raw[i]/h_TotalEvts_raw->GetBinContent(i+1);
      EvtSizeAvePD_raw[i] = TotSizeAvePD_raw[i]/h_TotalEvts_raw->GetBinContent(i+1);
    }
  }


  cout << endl << endl;

  //################################################################
  // Print the L1 Rates.... 

  cout << "__________________________________________________________________________________________________________________________ " << endl;
  cout << "                  TRIGGER STRING                                 |   Raw evts  | PreScale  | Scaled Evts  |     Rate       " << endl;
  cout << " --------------------------------------------------------------- | ----------- | --------  | -----------  | -------------  " << endl;

  for(int i=0; i<nTriggers_L1; i++){
    //if(PreScale_L1[i]>1e8)
      //continue;
    sprintf(saythis, "%63s", TrStrings_L1[i]);
    sprintf(saythis1,"%6d",  nEvts_L1_raw[i]);
    sprintf(saythis2,"%6d",  nEvts_L1_psc[i]);
    Double_t quickrate = Double_t(nEvts_L1_psc[i])*FullRate/Double_t(evtCount);
    if(quickrate>10.0)
      sprintf(saythis7,"%6d  ",  int(quickrate));
    else
      sprintf(saythis7,"  %6.1f",  quickrate);
    if(PreScale_L1[i]>199999)  sprintf(saythis3,"  2e5");
    else                       sprintf(saythis3,"%5.0d",Int_t(PreScale_L1[i]));
    cout << saythis << "  |   " << saythis1  << "    |   " << saythis3  << "   |   " << saythis2  << "     |   " << saythis7  << "   " << endl;
  }
  cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
  sprintf(saythis1,"%6d",  SumOfEvt_L1_raw);
  sprintf(saythis2,"%6d",  SumOfEvt_L1_psc);
  sprintf(saythis3,"%6d",  TotalEvt_L1_raw);
  sprintf(saythis4,"%6d",  TotalEvt_L1_psc);
  sprintf(saythis5,"%4.3f",  double(TotalEvt_L1_raw)/double(SumOfEvt_L1_raw));
  sprintf(saythis6,"%4.3f",  double(TotalEvt_L1_psc)/double(SumOfEvt_L1_psc));
  sprintf(saythis7,"%6d",  int(TotalEvt_L1_psc*FullRate/Double_t(evtCount)));
  cout << "                  SIMPLE SUM                                     |   " << saythis1  << "    |           |   " << saythis2  << "   " << endl;
  cout << "             ACTUAL AFTER OVERLAPS                               |   " << saythis3  << "    |           |   " << saythis4  << "    |    " << saythis7 << "   " << endl;
  cout << "             Ratio of simple_sum/total                           |~   " << saythis5  << "   ~|           |~  " << saythis6  << "  ~" << endl;
  cout << "___________________________________________________________________________________________________________________________" << endl;
  cout << endl;
  //
  //################################################################




  //################################################################
  // Print the HLT Rates.... 

  cout << endl;
  cout << "____________________________________________________________________________________________________________________________________________________   " << endl;
  cout << "                  TRIGGER STRING                                 |  NoPS Evts  | L1 pScale | Survive HLT | HLT pScale|  Rec Events |      Rate     |   " << endl;
  cout << " --------------------------------------------------------------- | fired L1&HLT| --------- | -after L1 ps| --------- | ----------- | ------------- |   " << endl;

  for(int i=0; i<nTriggersHlt; i++){
    if(PreScaleHlt[i]>1e8)
      continue;
    sprintf(saythis, "%60s", TrStringsHlt[i]);
    sprintf(saythis1,"%6d",  nEvtsHlt_ps1[i]);
    sprintf(saythis2,"%6d",  nEvtsHlt_raw[i]);
    sprintf(saythis3,"%6d",  nEvtsHlt_psc[i]);
    Double_t quickrate = Double_t(nEvtsHlt_psc[i])*FullRate/Double_t(evtCount);
    if(quickrate>10.0)
      sprintf(saythis7,"%6d  ",  int(quickrate));
    else
      sprintf(saythis7,"  %6.1f",  quickrate);
    if(PreScaleHlt[i]>199999)  sprintf(saythis5,"  2e5");
    else                       sprintf(saythis5,"%5d",Int_t(PreScaleHlt[i]));
    if(PreScale_L1[L1PathForHLT[i]]>199999)  sprintf(saythis6,"  2e5");
    else                                     sprintf(saythis6,"%5d",Int_t(PreScale_L1[L1PathForHLT[i]]));
    cout << saythis << "  |   " << saythis1  << "    |   " << saythis6  << "   |   " << saythis2  << "    |   " << saythis5  << "   |   " << saythis3 << "    |   " << saythis7 << "    |   " << endl;
    //cout << nEvtsHlt_ps1[i]/PreScale_L1[L1PathForHLT[i]] << " / " << nEvtsHlt_raw[i] << " = " << nEvtsHlt_ps1[i]/PreScale_L1[L1PathForHLT[i]]/nEvtsHlt_raw[i] << endl << endl;
  }
  cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
  sprintf(saythis1,"%6d",  SumOfEvtHlt_raw);
  sprintf(saythis2,"%4d",  SumOfEvtHlt_psc);
  sprintf(saythis3,"%6d",  TotalEvtHlt_raw);
  sprintf(saythis4,"%4d",  TotalEvtHlt_psc);
  sprintf(saythis5,"%4.3f",  double(TotalEvtHlt_raw)/double(SumOfEvtHlt_raw));
  sprintf(saythis6,"%4.3f",  double(TotalEvtHlt_psc)/double(SumOfEvtHlt_psc));
  sprintf(saythis7,"%6d",  int(TotalEvtHlt_psc*FullRate/evtCount));
  cout << "                     SIMPLE SUM                                                            |   " << saythis1  << "    |           |   " << saythis2  << "   " << endl;
  cout << "                ACTUAL AFTER OVERLAPS                                                      |   " << saythis3  << "    |           |   " << saythis4  << "    |    " << saythis7 << endl;
  cout << "                Ratio of simple_sum/total                                                  |~   " << saythis5  << "   ~|           |~  " << saythis6  << "  ~" << endl;
  cout << "_______________________________________________________________________________________________________________________________________________  " << endl;
  cout << endl;
  //################################################################
  // Print the L1 Rates.... 


  for(int i=0; i<nTriggersHlt; i++){
    h_tr_tr_raw     ->GetXaxis()->SetBinLabel(i+1,TrStringsHlt[i]);
    h_tr_tr_psc     ->GetXaxis()->SetBinLabel(i+1,TrStringsHlt[i]);
    h_tr_tr_raw     ->GetYaxis()->SetBinLabel(i+1,TrStringsHlt[i]);
    h_tr_tr_psc     ->GetYaxis()->SetBinLabel(i+1,TrStringsHlt[i]);
    h_tr_pd_raw   ->GetXaxis()->SetBinLabel(i+1,TrStringsHlt[i]);
    h_tr_pd_psc   ->GetXaxis()->SetBinLabel(i+1,TrStringsHlt[i]);
    h_RatesPerTr_psc->GetXaxis()->SetBinLabel(i+1,TrStringsHlt[i]);
    h_RatesPerTr_raw->GetXaxis()->SetBinLabel(i+1,TrStringsHlt[i]);
    h_PS_for_trigHlt->GetXaxis()->SetBinLabel(i+1,TrStringsHlt[i]);
  }
  for(int i=0; i<nTriggers_L1; i++){
    h_L1_L1_raw     ->GetXaxis()->SetBinLabel(i+1,TrStrings_L1[i]);
    h_L1_L1_psc     ->GetXaxis()->SetBinLabel(i+1,TrStrings_L1[i]);
    h_L1_L1_raw     ->GetYaxis()->SetBinLabel(i+1,TrStrings_L1[i]);
    h_L1_L1_psc     ->GetYaxis()->SetBinLabel(i+1,TrStrings_L1[i]);
    h_RatesPerL1_psc->GetXaxis()->SetBinLabel(i+1,TrStrings_L1[i]);
    h_RatesPerL1_raw->GetXaxis()->SetBinLabel(i+1,TrStrings_L1[i]);
    h_PS_for_trig_L1->GetXaxis()->SetBinLabel(i+1,TrStrings_L1[i]);
  }

  cout << endl << "At " << Int_t(FullRate/1000.0) << " kHz : " << endl;
  for(int i=0; i<nPrimaryDSet; i++){
    h_tr_pd_raw     ->GetYaxis()->SetBinLabel(i+1,PDStrings[i]);
    h_tr_pd_psc     ->GetYaxis()->SetBinLabel(i+1,PDStrings[i]);
    h_pd_pd_raw     ->GetXaxis()->SetBinLabel(i+1,PDStrings[i]);
    h_pd_pd_psc     ->GetXaxis()->SetBinLabel(i+1,PDStrings[i]);
    h_pd_pd_raw     ->GetYaxis()->SetBinLabel(i+1,PDStrings[i]);
    h_pd_pd_psc     ->GetYaxis()->SetBinLabel(i+1,PDStrings[i]);
    h_UniqueEvts    ->GetXaxis()->SetBinLabel(i+1,PDStrings[i]);
    h_UniqueEffs    ->GetXaxis()->SetBinLabel(i+1,PDStrings[i]);
    h_RatesPerPD_psc->GetXaxis()->SetBinLabel(i+1,PDStrings[i]);
    h_RatesPerPD_raw->GetXaxis()->SetBinLabel(i+1,PDStrings[i]);

    cout << PDStrings[i] << " has a rate of " << Int_t(h_RatesPerPD_psc->GetBinContent(i+1)) << "  Hz\tand has " << 
      h_UniqueEvts   ->GetBinContent(i+1) << "\tunique events out of " << 
      h_TotalEvts_psc->GetBinContent(i+1) << " \tfor and UniqueEff: " << 
      h_UniqueEffs   ->GetBinContent(i+1) << " . " << endl;
  }

  cout << endl;
  cout << "   -_-_-_-_-                       -_-_-_-_-" << endl;
  cout << "     -_-_-   EVENT SIZES ESTIMATES   -_-_-  " << endl;
  cout << "       -                               -    " << endl;

  cout << endl << "   ~~with current prescales:" << endl;
  for(int i=0; i<nPrimaryDSet; i++){
    cout << PDStrings[i] << " (" << Int_t(h_RatesPerPD_psc->GetBinContent(i+1)) << " Hz) \thas an estimated average event size " << 
      EvtSizeAvePD_psc[i] << " kb/evt  --(estimated max evt size ~ " << EvtSizeMaxPD_psc[i] << " kb/evt). " << endl;
  }
  cout << "     ----->" << endl;
  for(int i=0; i<nPrimaryDSet; i++){
    cout << PDStrings[i] << "               est. ave data rate:  " << h_RatesPerPD_psc->GetBinContent(i+1)*EvtSizeAvePD_psc[i] << "  kb/sec   (est. max ~  " << 
      h_RatesPerPD_psc->GetBinContent(i+1)*EvtSizeMaxPD_psc[i] << "  kb/sec). " << endl;
  }
  Double_t TotalDataRateAve = 0.0;
  Double_t TotalDataRateMax = 0.0;
  for(int i=0; i<nPrimaryDSet; i++){
    if(EvtSizeAvePD_psc[i]==EvtSizeAvePD_psc[i])
      TotalDataRateAve += h_RatesPerPD_psc->GetBinContent(i+1)*EvtSizeAvePD_psc[i];
    if(EvtSizeMaxPD_psc[i]==EvtSizeMaxPD_psc[i])
      TotalDataRateMax += h_RatesPerPD_psc->GetBinContent(i+1)*EvtSizeMaxPD_psc[i];
  }
  cout << "          -----> TOTAL DATA RATES:                                   " << TotalDataRateAve << "  kb/sec (ave)         " << TotalDataRateMax << "  kb/sec (max)." << endl;
  cout << endl << endl << "   ~~without prescales:" << endl;
  for(int i=0; i<nPrimaryDSet; i++){
    cout << PDStrings[i] << " (" << Int_t(h_RatesPerPD_raw->GetBinContent(i+1)) << " Hz) \thas estimated average event size " << 
      EvtSizeAvePD_raw[i] << " kb/evt  --(estimated max evt size ~ " << EvtSizeMaxPD_raw[i] << " kb/evt). " << endl;
  }


  Int_t TotalEventsStored = Int_t(h_RatesPerPD_psc->Integral(1,nPrimaryDSet)*Double_t(evtCount)/FullRate);
  Int_t RedundantEvents   = TotalEventsStored - h_nPDsFired->Integral(2,-1);

  cout << endl << endl;
  cout << " We stored a total of  " << TotalEventsStored << "  events." << endl;
  cout << " Events that were in some way redundant:  " << RedundantEvents << endl;
  cout << " If we had only 1 PD, we'd have stored  " << h_nPDsFired->Integral(2,-1) << "  events." << endl;
  if(h_nPDsFired->Integral(2,-1)>0)
    cout << " We are writing " << 100.0*RedundantEvents/h_nPDsFired->Integral(2,-1) << " \% too many events." << endl;

  cout << endl;
  cout << "_______________________________________________________________________" << endl;
  cout << "We examined " << evtCount << " / " << nentries << " events." << endl << endl;;


  TLine *line_PDEdges[nPrimaryDSet];
  TBox *DiagonalBox[nPrimaryDSet];
  for(int i=0; i<nPrimaryDSet; i++){
    line_PDEdges[i] = new TLine(0.5,i+0.5,nTriggersHlt+0.5,i+0.5);
    line_PDEdges[i]->SetLineWidth(2);
    if(i==0)
      DiagonalBox[i] = new TBox(0.5,i+0.5,PDIndexEdges[i]+0.5,i+1.5);
    else if(i<nPrimaryDSet-1)
      DiagonalBox[i] = new TBox(PDIndexEdges[i-1]+0.5,i+0.5,PDIndexEdges[i]+0.5,i+1.5);
    else
      DiagonalBox[i] = new TBox(PDIndexEdges[i-1]+0.5,i+0.5,nTriggersHlt+0.5,i+1.5);
    //sprintf(saythis,"DiagonalBox_%d",i);
    //DiagonalBox[i]->SetName(saythis);
    DiagonalBox[i]->SetFillStyle(0);
    DiagonalBox[i]->SetLineWidth(4);
    DiagonalBox[i]->SetLineColor(kRed);
  }
  TLine *line_TrEdges[nPrimaryDSet];
  for(int i=0; i<nPrimaryDSet; i++){
    line_TrEdges[i] = new TLine(PDIndexEdges[i]+0.5,0.5, PDIndexEdges[i]+0.5,nPrimaryDSet+0.5);
    line_TrEdges[i]->SetLineWidth(2);
  }
  gStyle->SetOptStat(0);
  //gStyle->SetPalette(kRainBow,0);
  gStyle->SetPalette(55,0);
  TCanvas *c1_raw = new TCanvas("c1_raw","c1_raw",1000,600);
  c1_raw->SetLeftMargin(0.18);
  c1_raw->SetLogz();
  h_tr_pd_raw->Draw("colz");
  for(int i=0; i<nPrimaryDSet; i++){
    line_TrEdges[i]->Draw("same");
    line_PDEdges[i]->Draw("same");
    DiagonalBox[i] ->Draw("same");
  }

  //calculate fractions inside h_tr_pd_raw
  std::vector<float> entries;
  double max = 0.;
  //h_TrPdFrRaw = dynamic_cast<TH2D*>(h_tr_pd_raw->Clone("hFractionsRaw"));
  h_TrPdFrRaw->Reset();
  for(int i = 1; i<=h_tr_pd_raw->GetXaxis()->GetNbins(); ++i) {
    max = 0.;
    for(int j = 1; j<=h_tr_pd_raw->GetYaxis()->GetNbins(); ++j) {
      double cur = h_tr_pd_raw->GetBinContent(i,j);
      entries.push_back(cur);
      if(cur>max) max = cur;
    }
    for(int j = 1; j<=h_tr_pd_raw->GetYaxis()->GetNbins(); ++j) {
      if(max>0.)
        h_TrPdFrRaw->SetBinContent(i,j,entries[j-1]/max);
      else
        h_TrPdFrRaw->SetBinContent(i,j,0.);
    }
    entries.clear();
  }
  gStyle->SetPaintTextFormat("4.2f");
  h_TrPdFrRaw->Draw("TEXT0 same");


  TCanvas *c1_psc = new TCanvas("c1_psc","c1_psc",1000,600);
  c1_psc->SetLeftMargin(0.18);
  c1_psc->SetLogz();
  h_tr_pd_psc->Draw("colz");
  for(int i=0; i<nPrimaryDSet; i++){
    line_TrEdges[i]->Draw("same");
    line_PDEdges[i]->Draw("same");
    DiagonalBox[i] ->Draw("same");
  }

  //calculate fractions inside h_tr_pd_psc
  entries.clear();
  max = 0.;
  //h_TrPdFrpsc = dynamic_cast<TH2D*>(h_tr_pd_psc->Clone("hFractionspsc"));
  h_TrPdFrPsc->Reset();
  for(int i = 1; i<=h_tr_pd_psc->GetXaxis()->GetNbins(); ++i) {
    max = 0.;
    for(int j = 1; j<=h_tr_pd_psc->GetYaxis()->GetNbins(); ++j) {
      double cur = h_tr_pd_psc->GetBinContent(i,j);
      entries.push_back(cur);
      if(cur>max) max = cur;
    }
    for(int j = 1; j<=h_tr_pd_psc->GetYaxis()->GetNbins(); ++j) {
      if(max>0.)
        h_TrPdFrPsc->SetBinContent(i,j,entries[j-1]/max);
      else
        h_TrPdFrPsc->SetBinContent(i,j,0.);
    }
    entries.clear();
  }
  gStyle->SetPaintTextFormat("4.2f");
  h_TrPdFrPsc->Draw("TEXT0 same");


  TCanvas *c1_rates = new TCanvas("c1_rates","c1_rates",800,800);
  c1_rates->Divide(2,2);
  c1_rates->cd(1);
  h_RatesPerPD_psc->Draw();
  c1_rates->cd(2);
  h_RatesPerPD_raw->Draw();
  c1_rates->cd(3);
  h_RatesPerTr_psc->Draw();
  c1_rates->cd(4);
  h_RatesPerTr_raw->Draw();


  TFile *fout = new TFile(outputfile,"RECREATE");
  for(int i=0; i<nTriggers_L1; i++){
    h_freq_L1_raw[i] ->Write();
    h_freq_L1_psc[i] ->Write();
  }
  for(int i=0; i<nTriggersHlt; i++){
    h_freq_HLT_raw[i] ->Write();
    h_freq_HLT_psc[i] ->Write();
  }

  h_L1_L1_raw->Write();
  h_L1_L1_psc->Write();
  h_tr_tr_raw->Write();
  h_tr_tr_psc->Write();
  h_tr_pd_raw->Write();
  h_tr_pd_psc->Write();
  h_pd_pd_raw->Write();
  h_pd_pd_psc->Write();
  h_TrPdFrRaw->Write();
  h_TrPdFrPsc->Write();
  h_UniqueEvts->Write();
  h_UniqueEffs->Write();
  h_nPDsFired->Write();
  h_TotalEvts_psc->Write();
  h_TotalEvts_raw->Write();
  for(int i=0; i<nPrimaryDSet; i++)
    h_PDs_NumOtherPDsFired[i]->Write();
  h_RatesPerPD_psc->Write();
  h_RatesPerPD_raw->Write();
  h_RatesPerL1_psc->Write();
  h_RatesPerL1_raw->Write();
  h_RatesPerTr_psc->Write();
  h_RatesPerTr_raw->Write();
  h_PS_for_trig_L1->Write();
  h_PS_for_trigHlt->Write();
  c1_raw->Write();
  c1_psc->Write();
  c1_rates->Write();
  fout->Close();


  cout << "Assumed Full Rate:  " << FullRate << " Hz" << endl;
  cout << "Accept Rate:  " << float(TotalEvtHlt_psc)/float(evtCount) << endl;
  cout << "Output Rate:  " << FullRate*float(TotalEvtHlt_psc)/float(evtCount) << endl;



}

