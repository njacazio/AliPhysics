#ifndef ALIHFMLVARHANDLER_H
#define ALIHFMLVARHANDLER_H

/* Copyright(c) 1998-2019, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//*************************************************************************
// \class AliHFMLVarHandler
// \brief helper class to handle a tree and variables for ML analyses
// \authors:
// F. Catalano, fabio.catalano@cern.ch
// F. Grosa, fabrizio.grosa@cern.ch
/////////////////////////////////////////////////////////////

#include <TTree.h>
#include <TString.h>

#include "AliAODTrack.h"
#include "AliAODRecoDecayHF.h"
#include "AliAODPidHF.h"

class AliHFMLVarHandler : public TObject
{
    public:
    
        enum candtype {
            kSignal = BIT(0),
            kBkg    = BIT(1),
            kPrompt = BIT(2),
            kFD     = BIT(3),
            kRefl   = BIT(4),
            kSignalWoQuark = BIT(5) //up to BIT(8) included for general flags, following BITS particle-specific
        };
    
        enum optpid {
            kNoPID,
            kRawPID,
            kNsigmaPID,
            kNsigmaCombPID,
            kNsigmaDetAndCombPID,
            kRawAndNsigmaPID
        };

        enum piddet {
            kTPC,
            kTOF,
            kCombTPCTOF // must be the last element in the enum
        };

        AliHFMLVarHandler();
        AliHFMLVarHandler(int PIDopt);
        virtual ~AliHFMLVarHandler();

        AliHFMLVarHandler(const AliHFMLVarHandler &source) = delete;
        AliHFMLVarHandler& operator=(const AliHFMLVarHandler &source) = delete;
        
        //core methods --> implemented in each derived class
        virtual TTree* BuildTree(TString /*name*/, TString /*title*/) {return nullptr;}
        virtual bool SetVariables(AliAODRecoDecayHF* /*cand*/, float /*bfield*/, int /*masshypo*/, AliAODPidHF* /*pidrespo*/) {return false;} //to be called for each candidate
        //to be called for each candidate
        void SetCandidateType(bool issignal, bool isbkg, bool isprompt, bool isFD, bool isreflected);
        void SetlsSignalWoQuark(bool isSignalWoQuark);
        void FillTree();
        
        //common methods
        void SetOptPID(int PIDopt) {fPidOpt = PIDopt;}
        void SetAddSingleTrackVars(bool add = true) {fAddSingleTrackVar = add;}
        void SetFillOnlySignal(bool fillopt = true) {fFillOnlySignal = fillopt;}

    protected:  
        //constant variables
        static const unsigned int knMaxProngs   = 4;
        static const unsigned int knMaxDet4Pid  = 2;
        static const unsigned int knMaxHypo4Pid = 3;

        const float kCSPEED = 2.99792457999999984e-02; // cm / ps

        //helper methods for derived clases (to be used in BuildTree and SetVariables functions)
        void AddCommonDmesonVarBranches();
        void AddSingleTrackBranches();
        void AddPidBranches(bool usePionHypo, bool useKaonHypo, bool useProtonHypo, bool useTPC, bool useTOF);
        bool SetSingleTrackVars(AliAODTrack* prongtracks[]);
        bool SetPidVars(AliAODTrack* prongtracks[], AliAODPidHF* pidrespo, bool usePionHypo, bool useKaonHypo, bool useProtonHypo, bool useTPC, bool useTOF);
    
        //utils methods
        float GetTOFmomentum(AliAODTrack* track, AliAODPidHF* pidrespo);

        TTree* fTreeVar = nullptr;                                     //!<! tree with variables
        int fPidOpt = kNsigmaPID;                                      /// option for PID variables
        bool fAddSingleTrackVar = false;                               /// add single-track variables
        bool fFillOnlySignal = false;                                  /// flag to enable only signal filling
        unsigned int fNProngs = -1;                                    /// number of prongs
        int fCandType = 0;                                             /// flag for candidate type (bit map above)
        float fInvMass = -999.;                                        /// candidate invariant mass
        float fPt = -999.;                                             /// candidate pt
        float fDecayLength = -999.;                                    /// candidate decay length
        float fDecayLengthXY = -999.;                                  /// candidate decay length in the transverse plane
        float fNormDecayLengthXY = -999.;                              /// candidate normalised decay length in the transverse plane
        float fCosP = -999.;                                           /// candidate cosine of pointing angle
        float fCosPXY = -999.;                                         /// candidate cosine of pointing angle in the transcverse plane
        float fImpParXY = -999.;                                       /// candidate impact parameter in the transverse plane
        float fDCA = -999.;                                            /// DCA of candidates prongs
        float fPtProng[knMaxProngs] = {-999., -999., -999., -999.};    /// prong pt
        float fTPCPProng[knMaxProngs] = {-999., -999., -999., -999.};  /// prong TPC momentum
        int fNTPCclsPidProng[knMaxProngs] = {-999, -999, -999, -999};  /// prong track number of clusters in TPC used for PID
        float fTOFPProng[knMaxProngs] = {-999., -999., -999., -999.};  /// prong TOF momentum
        float fTrackIntegratedLengthProng[knMaxProngs] = {-999., -999., -999., -999.};  /// prong track integrated lengths
        float fStartTimeResProng[knMaxProngs] = {-999., -999., -999., -999.};           /// prong track start time resolutions (for TOF)
        float fPIDNsigmaVector[knMaxProngs][knMaxDet4Pid+1][knMaxHypo4Pid];             /// PID nsigma variables
        float fPIDrawVector[knMaxProngs][knMaxDet4Pid];                                 /// raw PID variables
        
    /// \cond CLASSIMP
    ClassDef(AliHFMLVarHandler, 1); ///
    /// \endcond
};
#endif
