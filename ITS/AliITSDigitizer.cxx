/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
 
/*
$Log$
Revision 1.1  2001/11/27 16:27:28  nilsen
Adding AliITSDigitizer class to do merging and digitization . Based on the
TTask method. AliITSDigitizer class added to the Makefile and ITSLinkDef.h
file. The following files required minor changes. AliITS, added functions
SetHitsAddressBranch, MakeBranchInTreeD and modified MakeBranchD.
AliITSsimulationSDD.cxx needed a Tree indepenent way of returning back to
the original Root Directory in function Compress1D. Now it uses gDirectory.

*/

//Piotr.Skowronski@cern.ch :
//Corrections applied in order to compile (only) with new I/O and folder structure
//To be implemented correctly by responsible

#include <stdlib.h>
#include <iostream.h>
#include <TObjArray.h>
#include <TTree.h>
#include <TBranch.h>

#include <AliRun.h>
#include <AliRunDigitizer.h>
#include <AliLoader.h>
#include "AliITSDigitizer.h"
#include "AliITShit.h"
#include "AliITSmodule.h"
#include "AliITSsimulation.h"
#include "AliITSDetType.h"
#include "AliITSgeom.h"

ClassImp(AliITSDigitizer)

//______________________________________________________________________
AliITSDigitizer::AliITSDigitizer() : AliDigitizer(){
    // Default constructor. Assign fITS since it is never written out from
    // here. 
    // Inputs:
    //      Option_t * opt   Not used
    // Outputs:
    //      none.
    // Return:
    //      A blank AliITSDigitizer class.

    fITS = 0;
}
//______________________________________________________________________
AliITSDigitizer::AliITSDigitizer(AliRunDigitizer *mngr) : AliDigitizer(mngr){
    // Standard constructor. Assign fITS since it is never written out from
    // here. 
    // Inputs:
    //      Option_t * opt   Not used
    // Outputs:
    //      none.
    // Return:
    //      An AliItSDigitizer class.

    fITS = 0;
}
//______________________________________________________________________
AliITSDigitizer::~AliITSDigitizer(){
    // Default destructor. 
    // Inputs:
    //      Option_t * opt   Not used
    // Outputs:
    //      none.
    // Return:
    //      none.

    fITS = 0; // don't delete fITS. Done else where.
}

//______________________________________________________________________
Bool_t AliITSDigitizer::Init(){
    // Iniliztion 
    // Inputs:
    //      none.
    // Outputs:
    //      none.
    // Return:
    //      none.

//    if(GetHits()) fITS->fHits = new TClonesArray("AliITSHit",1000);
    return kTRUE;
}
//______________________________________________________________________
void AliITSDigitizer::Exec(Option_t* opt){
    // Main digitizing function. 
    // Inputs:
    //      Option_t * opt   list of subdetector to digitize. =0 all.
    // Outputs:
    //      none.
    // Return:
    //      none.
    Int_t size=0,ifls=0,trk=0,ntracks=0,h=0,nhit=0;
    TTree *treeH=0;
    TBranch *brchHits=0;
    AliITShit *itsHit=0;
    char name[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    char *all;
    const char *det[3] = {strstr(opt,"SPD"),strstr(opt,"SDD"),
                          strstr(opt,"SSD")};
    if(!det[0] && !det[1] && !det[2]) all = "All";
    else all = 0;
    AliITSsimulation *sim      = 0;
    AliITSDetType    *iDetType = 0;
    AliITSmodule     *mod      = 0;
    Int_t id=0,module=0,nfls=0,mask=0;
    static Bool_t setDef=kTRUE;

    AliRunLoader *inRL, *outRL;//in and out Run Loaders
    AliLoader *ingime, *outgime;// in and out ITSLoaders
   
   
    outRL = AliRunLoader::GetRunLoader(fManager->GetOutputFolderName());
    outgime = outRL->GetLoader("ITSLoader");

    if(!fITS) fITS = (AliITS*)(gAlice->GetDetector("ITS"));
    if(!(fITS->GetITSgeom())){
      Warning("Exec","Need ITS geometry to be properly defined first.");
      return; // need transformations to do digitization.
    } // end if !GetITSgeom()
    if (setDef) fITS->SetDefaultSimulation();
    setDef=kFALSE;
    sprintf(name,"%s",fITS->GetName());
    if(!GetModules()) {
      fITS->InitModules(0,size);
    } // end if

    nfls = GetManager()->GetNinputs();
    for(ifls=0;ifls<nfls;ifls++)
     {
      inRL = AliRunLoader::GetRunLoader(fManager->GetInputFolderName(ifls));
      ingime = inRL->GetLoader("ITSLoader");

      ingime->LoadHits("READ");//probably it is necessary to load them before
      treeH = ingime->TreeH();
      if(!(treeH && GetHits())) continue;
      brchHits = treeH->GetBranch(name);
      if(brchHits){
          GetHits()->Clear();
          fITS->SetHitsAddressBranch(brchHits);
      } else{
          Error("Exec","branch ITS not found");
      } // end if brchHits

      ntracks = (Int_t) treeH->GetEntries();
      for(trk=0;trk<ntracks;trk++){
          GetHits()->Clear();
          brchHits->GetEntry(trk);
          nhit = GetHits()->GetEntries();
          mask = GetManager()->GetMask(ifls);
          for(h=0;h<nhit;h++){
            itsHit = GetHit(h);
            mod = GetModule(itsHit->GetModule());
            id       = fITS->GetITSgeom()->GetModuleType(module);
            if (!all && !det[id]) continue;
            mod->AddHit(itsHit,trk+mask,h);
          } // end for h
      } // end for trk
    } // end for ifls

    // Digitize 
    fITS->MakeBranchInTreeD(outgime->TreeD());
    for(module=0;module<size;module++){
        id       = fITS->GetITSgeom()->GetModuleType(module);
        if (!all && !det[id]) continue;
        iDetType = fITS->DetType(id);
        sim      = (AliITSsimulation*)iDetType->GetSimulationModel();
        if (!sim) {
            Error("Exec","The simulation class was not instanciated!");
            exit(1);
        } // end if !sim
        mod      = GetModule(module);
        sim->DigitiseModule(mod,module,0);
        // fills all branches - wasted disk space
        outgime->TreeD()->Fill();
        fITS->ResetDigits();
    } // end for module
 
    fITS->ClearModules();
 
    outgime->TreeD()->GetEntries();
    outgime->WriteDigits("OVERWRITE");
    // reset tree - not necssary, tree is deleted when getting new event
}
