TBranch*  findBranch(const char* beginsWith, TTree* events) {
  TObjArray* branches=events->GetListOfBranches();
  for (int i=0; i<branches->GetEntries(); i++) {
    TBranch* br=(TBranch*)(branches->At(i));
    if (strstr(br->GetName(),beginsWith)==br->GetName()) {
      char name[1024];
      sprintf(name,"%sobj",br->GetName());
      br=(TBranch*)events->GetBranch(name);
      return br;
    }
  }
  return 0;
}

