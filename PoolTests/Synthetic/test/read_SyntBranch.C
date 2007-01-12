void read_SyntBranch() {
  gSystem->Load("libCintex");
  Cintex::Enable();
  gSystem->Load("libPoolTestsPoolTreeTests");
  TFile f("T1.root");
  TTree * t =  (TTree*)f.Get("Syn");
  t->Show(7);
  t->Scan("m_2:m_3","id<10");
  t->Scan("","id<5");
}
