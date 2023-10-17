

  Int_t nEvts_L1_raw[nTriggers_L1] = {0};
  Int_t nEvtsHlt_raw[nTriggersHlt] = {0};
  Int_t nEvtsHlt_ps1[nTriggersHlt] = {0};
  Int_t nEvts_L1_psc[nTriggers_L1] = {0};
  Int_t nEvtsHlt_psc[nTriggersHlt] = {0};
  Int_t TotalEvt_L1_raw = 0;
  Int_t TotalEvtHlt_raw = 0;
  Int_t TotalEvt_L1_psc = 0;
  Int_t TotalEvtHlt_psc = 0;
  Int_t SumOfEvt_L1_raw = 0;
  Int_t SumOfEvtHlt_raw = 0;
  Int_t SumOfEvt_L1_psc = 0;
  Int_t SumOfEvtHlt_psc = 0;
  Int_t FiredPD_raw[nPrimaryDSet] = {0};
  Int_t FiredPD_psc[nPrimaryDSet] = {0};
  Int_t FiredL1_raw[nTriggers_L1] = {0};
  Int_t FiredL1_psc[nTriggers_L1] = {0};
  Int_t FiredTr_raw[nTriggersHlt] = {0};
  Int_t FiredTr_psc[nTriggersHlt] = {0};

  Double_t TotSizeMaxPD_psc[nPrimaryDSet] = {0};
  Double_t TotSizeAvePD_psc[nPrimaryDSet] = {0};
  Double_t EvtSizeMaxPD_psc[nPrimaryDSet] = {0};
  Double_t EvtSizeAvePD_psc[nPrimaryDSet] = {0};
  Double_t TotSizeMaxPD_raw[nPrimaryDSet] = {0};
  Double_t TotSizeAvePD_raw[nPrimaryDSet] = {0};
  Double_t EvtSizeMaxPD_raw[nPrimaryDSet] = {0};
  Double_t EvtSizeAvePD_raw[nPrimaryDSet] = {0};

  TH2D *h_L1_L1_raw    = new TH2D("h_L1_L1_raw","h_L1_L1_raw",nTriggers_L1,0.5, nTriggers_L1+0.5, nTriggers_L1,0.5,nTriggers_L1+0.5);
  TH2D *h_L1_L1_psc    = new TH2D("h_L1_L1_psc","h_L1_L1_psc",nTriggers_L1,0.5, nTriggers_L1+0.5, nTriggers_L1,0.5,nTriggers_L1+0.5);
  TH2D *h_tr_tr_raw    = new TH2D("h_tr_tr_raw","h_tr_tr_raw",nTriggersHlt,0.5, nTriggersHlt+0.5, nTriggersHlt,0.5,nTriggersHlt+0.5);
  TH2D *h_tr_tr_psc    = new TH2D("h_tr_tr_psc","h_tr_tr_psc",nTriggersHlt,0.5, nTriggersHlt+0.5, nTriggersHlt,0.5,nTriggersHlt+0.5);
  TH2D *h_tr_pd_raw    = new TH2D("h_tr_pd_raw","h_tr_pd_raw",nTriggersHlt,0.5, nTriggersHlt+0.5, nPrimaryDSet,0.5,nPrimaryDSet+0.5);
  TH2D *h_tr_pd_psc    = new TH2D("h_tr_pd_psc","h_tr_pd_psc",nTriggersHlt,0.5, nTriggersHlt+0.5, nPrimaryDSet,0.5,nPrimaryDSet+0.5);
  TH2D *h_TrPdFrRaw    = new TH2D("h_TrPdFrRaw","h_TrPdFrRaw",nTriggersHlt,0.5, nTriggersHlt+0.5, nPrimaryDSet,0.5,nPrimaryDSet+0.5);
  TH2D *h_TrPdFrPsc    = new TH2D("h_TrPdFrPsc","h_TrPdFrPsc",nTriggersHlt,0.5, nTriggersHlt+0.5, nPrimaryDSet,0.5,nPrimaryDSet+0.5);
  TH2D *h_pd_pd_raw    = new TH2D("h_pd_pd_raw","h_pd_pd_raw",nPrimaryDSet,0.5, nPrimaryDSet+0.5, nPrimaryDSet,0.5,nPrimaryDSet+0.5);
  TH2D *h_pd_pd_psc    = new TH2D("h_pd_pd_psc","h_pd_pd_psc",nPrimaryDSet,0.5, nPrimaryDSet+0.5, nPrimaryDSet,0.5,nPrimaryDSet+0.5);

  TH1D *h_TotalEvts_psc  = new TH1D("h_TotalEvts_psc" ,"number of PS'ed events for each PD;Primary Dataset;",nPrimaryDSet, 0.5, nPrimaryDSet+0.5);
  TH1D *h_TotalEvts_raw  = new TH1D("h_TotalEvts_raw" ,"number of raw events for each PD;Primary Dataset;",  nPrimaryDSet, 0.5, nPrimaryDSet+0.5);
  TH1D *h_UniqueEvts     = new TH1D("h_UniqueEvts","number of unique events for each PD;Primary Dataset;",nPrimaryDSet, 0.5, nPrimaryDSet+0.5);
  TH1D *h_UniqueEffs     = new TH1D("h_UniqueEffs","Unique Efficiency for each PD;Primary Dataset;",nPrimaryDSet, 0.5, nPrimaryDSet+0.5);
  TH1D *h_RatesPerPD_psc = new TH1D("h_RatesPerPD_psc","h_RatesPerPD_psc",nPrimaryDSet, 0.5, nPrimaryDSet+0.5);
  TH1D *h_RatesPerPD_raw = new TH1D("h_RatesPerPD_raw","h_RatesPerPD_raw",nPrimaryDSet, 0.5, nPrimaryDSet+0.5);
  TH1D *h_RatesPerL1_psc = new TH1D("h_RatesPerL1_psc","h_RatesPerL1_psc",nTriggers_L1,0.5,nTriggers_L1+0.5);
  TH1D *h_RatesPerL1_raw = new TH1D("h_RatesPerL1_raw","h_RatesPerL1_raw",nTriggers_L1,0.5,nTriggers_L1+0.5);
  TH1D *h_RatesPerTr_psc = new TH1D("h_RatesPerTr_psc","h_RatesPerTr_psc",nTriggersHlt,0.5,nTriggersHlt+0.5);
  TH1D *h_RatesPerTr_raw = new TH1D("h_RatesPerTr_raw","h_RatesPerTr_raw",nTriggersHlt,0.5,nTriggersHlt+0.5);
  TH1D *h_nPDsFired      = new TH1D("h_nPDsFired" ,"number of PDs that fired; N_{PDs fired};N_{evts}" ,nPrimaryDSet+1, -0.5, nPrimaryDSet+0.5);
  TH1D *h_PS_for_trig_L1 = new TH1D("h_PS_for_trig_L1","h_PS_for_trig_L1;trigger;prescale",nTriggers_L1,0.5,nTriggers_L1+0.5);
  TH1D *h_PS_for_trigHlt = new TH1D("h_PS_for_trigHlt","h_PS_for_trigHlt;trigger;prescale",nTriggersHlt,0.5,nTriggersHlt+0.5);
  TH1D* h_PDs_NumOtherPDsFired[nPrimaryDSet];
  for(int i=0; i<nPrimaryDSet; i++){
    sprintf(saythis,"h_PDs_NumOtherPDsFired_%d",i);
    h_PDs_NumOtherPDsFired[i] = new TH1D(saythis,saythis,nPrimaryDSet+1, -0.5, nPrimaryDSet+0.5);
    sprintf(saythis,"Number of Other PDs fired by %s;number of other PDs recorded;N_{evts}",PDStrings[i]);
    h_PDs_NumOtherPDsFired[i]->SetTitle(saythis);
  }
  sprintf(saythis,"Rates after prescales, %d kHz;Primary Dataset;Rate (Hz)",Int_t(FullRate/1000.0));
  h_RatesPerPD_psc->SetTitle(saythis);
  sprintf(saythis,"Rates with PS=1, %d kHz;Primary Dataset;Rate (Hz)",Int_t(FullRate/1000.0));
  h_RatesPerPD_raw->SetTitle(saythis);
  sprintf(saythis,"Rates after prescales, %d kHz;HLT Trigger;Rate (Hz)",Int_t(FullRate/1000.0));
  h_RatesPerTr_psc->SetTitle(saythis);
  sprintf(saythis,"Rates with PS=1, %d kHz;HLT Trigger;Rate (Hz)",Int_t(FullRate/1000.0));
  h_RatesPerTr_raw->SetTitle(saythis);
  sprintf(saythis,"Rates after prescales, %d kHz;L1 Trigger;Rate (Hz)",Int_t(FullRate/1000.0));
  h_RatesPerL1_psc->SetTitle(saythis);
  sprintf(saythis,"Rates with PS=1, %d kHz;L1 Trigger;Rate (Hz)",Int_t(FullRate/1000.0));
  h_RatesPerL1_raw->SetTitle(saythis);

  TH1F* h_freq_L1_raw[nTriggers_L1];
  TH1F* h_freq_L1_psc[nTriggers_L1];
  TH1F* h_freq_HLT_raw[nTriggersHlt];
  TH1F* h_freq_HLT_psc[nTriggersHlt];

  for(int i=0; i<nTriggers_L1; i++){
    h_PS_for_trig_L1->SetBinContent(i+1,PreScale_L1[i]);

    sprintf(saythis,"h_freq_L1_raw_%d",i);
    h_freq_L1_raw[i] = new TH1F(saythis,TrStrings_L1[i],10000,0.5,10000.5);
    h_freq_L1_raw[i] ->GetXaxis()->SetTitle("N^{raw}_{evts} since trigger last fired (L1)");
    h_freq_L1_raw[i] ->SetLineColor(1);

    sprintf(saythis,"h_freq_L1_psc_%d",i);
    h_freq_L1_psc[i] = new TH1F(saythis,TrStrings_L1[i],10000,0.5,10000.5);
    h_freq_L1_psc[i] ->GetXaxis()->SetTitle("N^{psc}_{evts} since trigger last fired (L1)");
    h_freq_L1_psc[i] ->SetLineColor(2);
  }

  for(int i=0; i<nTriggersHlt; i++){
    h_PS_for_trigHlt->SetBinContent(i+1,PreScaleHlt[i]);

    sprintf(saythis,"h_freq_HLT_raw_%d",i);
    h_freq_HLT_raw[i] = new TH1F(saythis,TrStringsHlt[i],10000,0.5,10000.5);
    h_freq_HLT_raw[i] ->GetXaxis()->SetTitle("N^{raw}_{evts} since trigger last fired (HLT)");
    h_freq_HLT_raw[i] ->SetLineColor(4);

    sprintf(saythis,"h_freq_HLT_psc_%d",i);
    h_freq_HLT_psc[i] = new TH1F(saythis,TrStringsHlt[i],10000,0.5,10000.5);
    h_freq_HLT_psc[i] ->GetXaxis()->SetTitle("N^{psc}_{evts} since trigger last fired (HLT)");
    h_freq_HLT_psc[i] ->SetLineColor(8);
  }

  cout << endl;
  cout << "input file   " << inputfile << endl;
  cout << "output file  " << outputfile << endl;
  cout << "Assumed full rate  " << int(FullRate/1000.0) << " kHz" << endl;
  cout << endl;


  cout << endl << "__________________________________________________________________________________________________________" << endl;
  cout <<         "--------------------------------  L1's corresponding to HLT's  -------------------------------------------" << endl;
  for(int i=0; i<nTriggersHlt; i++)
    for(int j=0; j<nTriggers_L1; j++)
      if(strcmp(L1StringForHlt[i],TrStrings_L1[j])==0){
        L1PathForHLT[i] = j;
        cout << j << "	" << TrStrings_L1[j] << "  -->  " << i << " " << TrStringsHlt[i] << " " << EvtSizeHlt[i] << " kb/evt" << endl;
      }
  cout << endl << "__________________________________________________________________________________________________________" << endl;
  cout << endl;

  cout << "_______________________________________________________" << endl;
  cout << "----------    HLT's corresponding to L1's    ----------" << endl;
  for(int j=0; j<nTriggers_L1; j++){
    cout << j << " " << TrStrings_L1[j] << endl;
    for(int i=0; i<nTriggersHlt; i++){
      if(L1PathForHLT[i]==j)
        cout << "    " << i << "	" << TrStringsHlt[i] << endl;
    }
  }
  cout << "_______________________________________________________" << endl;
  cout << endl;


  cout << endl << "__________________________________________________________________________________________________________" << endl;
  cout <<         "------------------------------------- Primary Datasets ---------------------------------------------------" << endl;
  int testj = 0;
  PDForHltID[0] = 0;
  for(int i=1; i<nTriggersHlt; i++){
    if(strcmp(PDForHlt[i],PDStrings[testj])!=0){
      PDIndexEdges[testj] = i;
      if(testj==0)
        cout << PDStrings[testj] << "  has " << i  << " triggers. " << endl;
      else
        cout << PDStrings[testj] << "  has " << PDIndexEdges[testj]-PDIndexEdges[testj-1]  << " triggers. " << endl;
      testj++;
    }
    PDForHltID[i] = testj;
  }
  cout << PDStrings[testj] << "  has " << nTriggersHlt-PDIndexEdges[testj-1]  << " triggers. " << endl;
  cout << endl << "__________________________________________________________________________________________________________" << endl;
  cout << endl;


  Long64_t last_jentry_L1_raw[nTriggers_L1]  = {0};
  Long64_t last_jentry_L1_psc[nTriggers_L1]  = {0};
  Long64_t last_jentry_HLT_raw[nTriggersHlt] = {0};
  Long64_t last_jentry_HLT_psc[nTriggersHlt] = {0};

  Long64_t evtCount=0;
  Long64_t nbytes = 0, nb = 0;
  for(Long64_t jentry=0; jentry<nentries; jentry++){
  //for(Long64_t jentry=0; jentry<100; jentry++){
    Long64_t ientry = LoadTree(jentry);
    if(ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    if(Int_t(jentry)%Int_t(0.1*nentries) == 0)
      cout << endl << " evt " << jentry << " / " << nentries << "  (" << 100.0*Double_t(jentry)/Double_t(nentries) << "%) ";
    else if(Int_t(jentry)%Int_t(0.01*nentries) == 0)
      cout << "."<< flush;


