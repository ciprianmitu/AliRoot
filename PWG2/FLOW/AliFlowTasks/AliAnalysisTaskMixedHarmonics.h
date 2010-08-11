/* 
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. 
 * See cxx source for full Copyright notice 
 * $Id$ 
 */

/**************************************
 * analysis task for mixed harmomics  * 
 *                                    * 
 * authors: Naomi van der Kolk        *
 *           (kolk@nikhef.nl)         *  
 *          Raimond Snellings         *
 *           (snelling@nikhef.nl)     * 
 *          Ante Bilandzic            *
 *           (anteb@nikhef.nl)        * 
 * ***********************************/

#ifndef ALIANALYSISTASKMIXEDHARMONICS_H
#define ALIANALYSISTASKMIXEDHARMONICS_H

#include "TString.h"
#include "AliAnalysisTaskSE.h"

class TList;
class AliFlowEventSimple;
class AliFlowAnalysisWithMixedHarmonics;

//================================================================================================================

class AliAnalysisTaskMixedHarmonics : public AliAnalysisTaskSE{
 public:
  AliAnalysisTaskMixedHarmonics();
  AliAnalysisTaskMixedHarmonics(const char *name, Bool_t useParticleWeights=kFALSE);
  virtual ~AliAnalysisTaskMixedHarmonics(){}; 
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t *option);
  virtual void   Terminate(Option_t *);
  
  // common:
  void SetHarmonic(Int_t const h) {this->fHarmonic = h;};
  Int_t GetHarmonic() const {return this->fHarmonic;}; 
  void SetNoOfMultipicityBins(Int_t const nomb) {this->fNoOfMultipicityBins = nomb;};
  Int_t GetNoOfMultipicityBins() const {return this->fNoOfMultipicityBins;};   
  void SetMultipicityBinWidth(Double_t const mbw) {this->fMultipicityBinWidth = mbw;};
  Double_t GetMultipicityBinWidth() const {return this->fMultipicityBinWidth;};   
  void SetMinMultiplicity(Double_t const mm) {this->fMinMultiplicity = mm;};
  Double_t GetMinMultiplicity() const {return this->fMinMultiplicity;}; 
  void SetOppositeChargesPOI(Bool_t const ocp) {this->fOppositeChargesPOI = ocp;};
  Bool_t GetOppositeChargesPOI() const {return this->fOppositeChargesPOI;}; 
  void SetEvaluateDifferential3pCorrelator(Bool_t const ed3pc) {this->fEvaluateDifferential3pCorrelator = ed3pc;};
  Bool_t GetEvaluateDifferential3pCorrelator() const {return this->fEvaluateDifferential3pCorrelator;};       
  void SetCorrectForDetectorEffects(Bool_t const cfde) {this->fCorrectForDetectorEffects = cfde;};
  Bool_t GetCorrectForDetectorEffects() const {return this->fCorrectForDetectorEffects;}; 
  // particle weights:
  void SetUsePhiWeights(Bool_t const uPhiW) {this->fUsePhiWeights = uPhiW;};
  Bool_t GetUsePhiWeights() const {return this->fUsePhiWeights;};
  void SetUsePtWeights(Bool_t const uPtW) {this->fUsePtWeights = uPtW;};
  Bool_t GetUsePtWeights() const {return this->fUsePtWeights;};
  void SetUseEtaWeights(Bool_t const uEtaW) {this->fUseEtaWeights = uEtaW;};
  Bool_t GetUseEtaWeights() const {return this->fUseEtaWeights;};
 
 private:
  AliAnalysisTaskMixedHarmonics(const AliAnalysisTaskMixedHarmonics& aatmh);
  AliAnalysisTaskMixedHarmonics& operator=(const AliAnalysisTaskMixedHarmonics& aatmh);
  
  AliFlowEventSimple *fEvent; // the input event
  AliFlowAnalysisWithMixedHarmonics *fMH; // mixed harmonics object
  TList *fListHistos; // collection of output 
  // common:  
  Int_t fHarmonic; // integer n in cos[n(phi1+phi2-2phi3)]
  Int_t fNoOfMultipicityBins; // number of multiplicity bins
  Double_t fMultipicityBinWidth; // width of multiplicity bin
  Double_t fMinMultiplicity; // minimal multiplicity
  Bool_t fOppositeChargesPOI; // two POIs, psi1 and psi2, in correlator <<cos[psi1+psi2-2phi3)]>> will be taken with opposite charges 
  Bool_t fEvaluateDifferential3pCorrelator; // evaluate <<cos[psi1+psi2-2phi3)]>>, where psi1 and psi2 are two POIs      
  Bool_t fCorrectForDetectorEffects; // correct 3-p correlator for detector effects
  // particle weights:
  Bool_t fUseParticleWeights; // use any particle weights
  Bool_t fUsePhiWeights; // use phi weights
  Bool_t fUsePtWeights; // use pt weights
  Bool_t fUseEtaWeights; // use eta weights  
  TList *fWeightsList; // list with weights
  
  ClassDef(AliAnalysisTaskMixedHarmonics, 1); 
};

//================================================================================================================

#endif











