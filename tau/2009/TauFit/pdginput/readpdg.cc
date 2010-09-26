#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "TROOT.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "TMath.h"
#include "TPostScript.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TKey.h"
#include "TLine.h"
#include "TBox.h"
#include "TArrow.h"
#include "TEllipse.h"
#include "TSpline.h"
#include "TF1.h"
#include "TH2D.h"
#include "TString.h"
#include "TCut.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TColor.h"
#include "TPaveText.h"
#include "Riostream.h"
#include "TMatrixD.h"
#include "TMatrixDLazy.h"
#include "TVectorD.h"
#include "TDecompLU.h"
#include "TDecompSVD.h"

enum e_basegammanames {
  M_GAMMA3 ,
  M_GAMMA5 ,
  M_GAMMA9 ,
  M_GAMMA10 ,
  M_GAMMA14 ,
  M_GAMMA16 ,
  M_GAMMA20 ,
  M_GAMMA23 ,
  M_GAMMA27 ,
  M_GAMMA28 ,
  M_GAMMA30 ,
  M_GAMMA35 ,
  M_GAMMA37 ,
  M_GAMMA40 ,
  M_GAMMA42 ,
  M_GAMMA47 ,
  M_GAMMA48 ,
  M_GAMMA62 ,
  M_GAMMA70 ,
  M_GAMMA77 ,
  M_GAMMA78 ,
  M_GAMMA85 ,
  M_GAMMA89 ,
  M_GAMMA93 ,
  M_GAMMA94 ,
  M_GAMMA104 ,
  M_GAMMA126 ,
  M_GAMMA128 ,
  M_GAMMA150 ,
  M_GAMMA152 ,
  M_GAMMA103
};

using namespace std;
// ----------------------------------------------------------------------
int main(int argc, char* argv[]){
  //Argument variables
  Int_t uconstrain  = (argc>1) ? atoi(argv[1]) : 1; // 1: unitarity constrained; 0 : not constrained
  Int_t doalephhcorr = (argc>2) ? atoi(argv[2]) : 1; // 1: do aleph hcorr; 0: dont
  //
  string sconstrain = (uconstrain) ? "constrained" : "unconstrained";
  string salephhcorr = (doalephhcorr) ? "_aleph_hcorr" : "";
  //
  gSystem->Load("libMatrix");
  //
  int ipar,iimeas,jjmeas,jpar;
  int i,j;
  // INPUT PARAMETERS
  // GAMMA PARMETER SEED TITLE //BASE = NQUAN (in COMBOS)
  // RESULTS FOR PARAMETERS
  // ----- -------- ---- ----- 
  const int nbase=31;
  vector<int> basequan;
  vector<int> basegamma;
  vector<int> baseparm;
  double      baseseed[nbase];
  string      basenode[nbase];
  string      basetitle[nbase];
  //
  int ibase = 0;
  ifstream ifsbase("base_def.txt") ;
  if (!ifsbase.good()) {cout << "Cannot open input file : base_def.txt" << endl ; exit(1) ;}
  char buffer[256]; 
  while(ifsbase.good()) {
    if (ifsbase.eof()) break;
    char firstch(' ') ; ifsbase.get(firstch) ;
    if (firstch=='#'||firstch=='\n') { // Skip such lines
      ifsbase.ignore(256,'\n') ;
    } else if (firstch=='*') {  // measurement line
      int dummy_int;       
      ifsbase >> dummy_int ; basequan.push_back(dummy_int);
      ifsbase >> dummy_int ; basegamma.push_back(dummy_int);
      ifsbase >> dummy_int ; baseparm.push_back(dummy_int);
      ifsbase >> basenode[ibase];
      ifsbase >> baseseed[ibase];
      ifsbase.getline(buffer,256,'\n');
      string stringbuffer=string(buffer);
      basetitle[ibase]="";
      bool first=false;
      for (string::iterator si=stringbuffer.begin();si!=stringbuffer.end();++si){
	if (13 == (int)*si) break; // ^M is 13
	if (' '!=*si) first=true;
	if (first) {
	  if (' ' == *si && ' ' == *(si+1) ) break;
	  basetitle[ibase]+=*si;
	}
      }
      // cout << basequan[ibase] << " " << basegamma[ibase] << " " << baseparm[ibase] << " " << basenode[ibase] << " " << baseseed[ibase] << " " << basetitle[ibase] << endl;
      ++ibase;
    }
  }
  //
  // From previous iteration of combos.log
  ibase=-1;
  baseseed[++ibase] = /*           M_GAMMA3    */  0.1735575 ; //+/-      0.0004626 +/-      0.0000000 Tot Err:     0.0004626 Check Sys:     0.0000000
  baseseed[++ibase] = /*           M_GAMMA5    */  0.1784136 ; //+/-      0.0004788 +/-      0.0000000 Tot Err:     0.0004788 Check Sys:     0.0000000
  baseseed[++ibase] = /*           M_GAMMA9    */  0.1090047 ; //+/-      0.0006175 +/-      0.0000000 Tot Err:     0.0006175 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA10    */  0.0069093 ; //+/-      0.0002188 +/-      0.0000000 Tot Err:     0.0002188 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA14    */  0.2549936 ; //+/-      0.0009233 +/-      0.0000000 Tot Err:     0.0009233 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA16    */  0.0045244 ; //+/-      0.0002645 +/-      0.0000000 Tot Err:     0.0002645 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA20    */  0.0924794 ; //+/-      0.0009745 +/-      0.0000000 Tot Err:     0.0009745 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA23    */  0.0005801 ; //+/-      0.0002251 +/-      0.0000000 Tot Err:     0.0002251 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA27    */  0.0104186 ; //+/-      0.0007060 +/-      0.0000000 Tot Err:     0.0007060 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA28    */  0.0004181 ; //+/-      0.0002186 +/-      0.0000000 Tot Err:     0.0002186 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA30    */  0.0010186 ; //+/-      0.0003920 +/-      0.0000000 Tot Err:     0.0003920 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA35    */  0.0089618 ; //+/-      0.0003671 +/-      0.0000000 Tot Err:     0.0003671 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA37    */  0.0015298 ; //+/-      0.0001606 +/-      0.0000000 Tot Err:     0.0001606 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA40    */  0.0037774 ; //+/-      0.0003676 +/-      0.0000000 Tot Err:     0.0003676 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA42    */  0.0015407 ; //+/-      0.0001995 +/-      0.0000000 Tot Err:     0.0001995 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA47    */  0.0002411 ; //+/-      0.0000516 +/-      0.0000000 Tot Err:     0.0000516 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA48    */  0.0011208 ; //+/-      0.0002454 +/-      0.0000000 Tot Err:     0.0002454 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA62    */  0.0898792 ; //+/-      0.0006003 +/-      0.0000000 Tot Err:     0.0006003 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA70    */  0.0268568 ; //+/-      0.0006932 +/-      0.0000000 Tot Err:     0.0006932 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA77    */  0.0009086 ; //+/-      0.0003595 +/-      0.0000000 Tot Err:     0.0003595 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA78    */  0.0002233 ; //+/-      0.0000499 +/-      0.0000000 Tot Err:     0.0000499 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA85    */  0.0033354 ; //+/-      0.0002232 +/-      0.0000000 Tot Err:     0.0002232 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA89    */  0.0007347 ; //+/-      0.0001157 +/-      0.0000000 Tot Err:     0.0001157 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA93    */  0.0015313 ; //+/-      0.0000699 +/-      0.0000000 Tot Err:     0.0000699 Check Sys:     0.0000000
  baseseed[++ibase] = /*          M_GAMMA94    */  0.0000609 ; //+/-      0.0000183 +/-      0.0000000 Tot Err:     0.0000183 Check Sys:     0.0000000
  baseseed[++ibase] = /*         M_GAMMA104    */  0.0001807 ; //+/-      0.0000260 +/-      0.0000000 Tot Err:     0.0000260 Check Sys:     0.0000000
  baseseed[++ibase] = /*         M_GAMMA126    */  0.0017735 ; //+/-      0.0002354 +/-      0.0000000 Tot Err:     0.0002354 Check Sys:     0.0000000
  baseseed[++ibase] = /*         M_GAMMA128    */  0.0002679 ; //+/-      0.0000632 +/-      0.0000000 Tot Err:     0.0000632 Check Sys:     0.0000000
  baseseed[++ibase] = /*         M_GAMMA150    */  0.0198809 ; //+/-      0.0006285 +/-      0.0000000 Tot Err:     0.0006285 Check Sys:     0.0000000
  baseseed[++ibase] = /*         M_GAMMA152    */  0.0040670 ; //+/-      0.0004175 +/-      0.0000000 Tot Err:     0.0004175 Check Sys:     0.0000000
  baseseed[++ibase] = 1- 0.9991898;
  
  //
  // INPUT NODES
  // NODE# NODE    EQ ADJ. UN. COUNT PAR_CODE PARAM.  SUM COEFFICIENT COEFF-EXTRA  
  // ----- ------- -- -------- ----- -------- ------- --- ----------- ------------ 
  const int      nnode=95;
  vector<string> nodegammaname;
  vector<string> nodename;
  int            node_num_npar[nnode];
  vector<int>    node_num_parm[nnode];
  vector<double> node_num_coef[nnode];
  int            node_den_npar[nnode];
  vector<int>    node_den_parm[nnode];
  vector<double> node_den_coef[nnode];
  string         nodetitle[nnode];
  int inode=0,jnode=0;
  //
  nodegammaname.push_back("Gamma128");     
  nodename.push_back("S035B20");    
  nodetitle[inode]="G(eta K- nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(1.        );              
  ++inode;  //1
  nodegammaname.push_back("Gamma19by13");  
  nodename.push_back("S035B21"); 
  nodetitle[inode]="G(h- 2pi0 nu(tau) (ex.K0))/G(h- pi0 nu(tau))";
  node_num_npar[inode]=2; node_den_npar[inode]=2;
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(201);       node_num_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(16 );       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(182);       node_den_coef[inode].push_back(1.        );               
  ++inode;  //2
  nodegammaname.push_back("Gamma26by13"); 
  nodename.push_back("S035B22"); 
  nodetitle[inode]="G(h- 3pi0 nu(tau))/G(h- pi0 nu(tau))";
  node_num_npar[inode]=5; node_den_npar[inode]=2;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(3.2200E-01);              
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(16 );       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(182);       node_den_coef[inode].push_back(1.        );               
  ++inode; //3
  nodegammaname.push_back("Gamma30"); 
  nodename.push_back("S035B23"); 
  nodetitle[inode]="G(h- 4pi0 nu(tau) (ex.K0,eta))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );              
  ++inode; //4
  nodegammaname.push_back("Gamma76by54"); 
  nodename.push_back("S035B25"); 
  nodetitle[inode]="G(h- h- h+ 2pi0 nu(tau) (ex.K0))/G(h- h- h+ >=0 neutrals >=0K(L)0 nu(tau) )";
  node_num_npar[inode]=3; node_den_npar[inode]=18;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(8.8800E-01);              
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(2.2600E-01);               
  node_den_parm[inode].push_back(109);       node_den_coef[inode].push_back(2.8500E-01);               
  node_den_parm[inode].push_back(113);       node_den_coef[inode].push_back(9.1010E-01);               
  node_den_parm[inode].push_back(117);       node_den_coef[inode].push_back(3.4310E-01);               
  node_den_parm[inode].push_back(118);       node_den_coef[inode].push_back(3.4310E-01);               
  node_den_parm[inode].push_back(119);       node_den_coef[inode].push_back(3.4310E-01);               
  node_den_parm[inode].push_back(204);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(214);       node_den_coef[inode].push_back(4.3070E-01);               
  node_den_parm[inode].push_back(216);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(240);       node_den_coef[inode].push_back(6.8610E-01);               
  node_den_parm[inode].push_back(247);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(259);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(260);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(263);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(285);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(5  );       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(58 );       node_den_coef[inode].push_back(2.8500E-01);               
  node_den_parm[inode].push_back(62 );       node_den_coef[inode].push_back(3.4310E-01);               
  node_den_parm[inode].push_back(8  );       node_den_coef[inode].push_back(9.1010E-01);               
  ++inode;//5
  nodegammaname.push_back("Gamma152by54"); 
  nodename.push_back("S035B26"); 
  nodetitle[inode]="G(h- omega pi0 nu(tau))/G(h- h- h+ >=0 neutrals >=0K(L)0 nu(tau) )";
  node_num_npar[inode]=1; node_den_npar[inode]=18;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(1.        );              
  node_den_parm[inode].push_back(109);       node_den_coef[inode].push_back(2.8500E-01);               
  node_den_parm[inode].push_back(113);       node_den_coef[inode].push_back(9.1010E-01);               
  node_den_parm[inode].push_back(117);       node_den_coef[inode].push_back(3.4310E-01);               
  node_den_parm[inode].push_back(118);       node_den_coef[inode].push_back(3.4310E-01);               
  node_den_parm[inode].push_back(119);       node_den_coef[inode].push_back(3.4310E-01);               
  node_den_parm[inode].push_back(204);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(214);       node_den_coef[inode].push_back(4.3070E-01);               
  node_den_parm[inode].push_back(216);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(240);       node_den_coef[inode].push_back(6.8610E-01);               
  node_den_parm[inode].push_back(247);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(259);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(260);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(263);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(285);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(5  );       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(58 );       node_den_coef[inode].push_back(2.8500E-01);               
  node_den_parm[inode].push_back(62 );       node_den_coef[inode].push_back(3.4310E-01);               
  node_den_parm[inode].push_back(8  );       node_den_coef[inode].push_back(9.1010E-01);               
  ++inode;//6
  nodegammaname.push_back("Gamma152by76"); 
  nodename.push_back("S035B27"); 
  nodetitle[inode]="G(h- omega pi0 nu(tau))/G(h- h- h+ 2pi0 nu(tau) (ex.K0))";
  node_num_npar[inode]=1; node_den_npar[inode]=3;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(1.        );              
  node_den_parm[inode].push_back(113);       node_den_coef[inode].push_back(8.8800E-01);               
  node_den_parm[inode].push_back(216);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(58 );       node_den_coef[inode].push_back(2.2600E-01);               
  ++inode;//7
  nodegammaname.push_back("Gamma16"); 
  nodename.push_back("S035B29"); 
  nodetitle[inode]="G(K- pi0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(182);       node_num_coef[inode].push_back(1.        );              
  ++inode;//8
  nodegammaname.push_back("Gamma23"); 
  nodename.push_back("S035B30"); 
  nodetitle[inode]="G(K- 2pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );              
  ++inode;//9
  nodegammaname.push_back("Gamma28"); 
  nodename.push_back("S035B31"); 
  nodetitle[inode]="G(K- 3pi0 nu(tau) (ex.K0, eta))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );              
  ++inode;//10
  nodegammaname.push_back("Gamma35"); 
  nodename.push_back("S035B32"); 
  nodetitle[inode]="G(pi- Kbar0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(1.        );              
  ++inode;//11
  nodegammaname.push_back("Gamma40"); 
  nodename.push_back("S035B33"); 
  nodetitle[inode]="G(pi- Kbar0 pi0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(1.        );              
  ++inode;//12
  nodegammaname.push_back("Gamma42"); 
  nodename.push_back("S035B34"); 
  nodetitle[inode]="G(K- K0 pi0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.        );              
  ++inode;//13
  nodegammaname.push_back("Gamma92"); 
  nodename.push_back("S035B37"); 
  nodetitle[inode]="G(K- K+ pi- >=0 neut.  nu(tau))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  ++inode;//14
  nodegammaname.push_back("Gamma33"); 
  nodename.push_back("S035B43"); 
  nodetitle[inode]="G(K(S)0 (particles)- nu(tau))/G(total)";
  node_num_npar[inode]=6; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(5.0000E-01);              
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(5.0000E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(5.0000E-01);               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(240);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(5.0000E-01);               
  ++inode;//15
  nodegammaname.push_back("Gamma106"); 
  nodename.push_back("S035B45"); 
  nodetitle[inode]="G(( 5pi )- nu(tau))/G(total)";
  node_num_npar[inode]=6; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(8.8800E-01);               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(3  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(5.5300E-01);               
  ++inode;//16
  nodegammaname.push_back("Gamma46"); 
  nodename.push_back("S035B51"); 
  nodetitle[inode]="G(pi- K0 Kbar0 nu(tau))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(2.0000E+00);              
  node_num_parm[inode].push_back(240);       node_num_coef[inode].push_back(1.        );               
  ++inode;//17
  nodegammaname.push_back("Gamma66"); 
  nodename.push_back("S035B53"); 
  nodetitle[inode]="G(h- h- h+ pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=6; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.2600E-01);              
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(1.7000E-02);               
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(8.8800E-01);               
  ++inode;//18
  nodegammaname.push_back("Gamma67"); 
  nodename.push_back("S035B54"); 
  nodetitle[inode]="G(h- h- h+ pi0 nu(tau) (ex. K0, omega))/G(total)";
  node_num_npar[inode]=4; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.2600E-01);              
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  ++inode;//19
  nodegammaname.push_back("Gamma20"); 
  nodename.push_back("S035B55"); 
  nodetitle[inode]="G(pi- 2pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(201);       node_num_coef[inode].push_back(1.        );              
  ++inode;//20
  nodegammaname.push_back("Gamma27"); 
  nodename.push_back("S035B56"); 
  nodetitle[inode]="G(pi- 3pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );              
  ++inode;//21
  nodegammaname.push_back("Gamma78"); 
  nodename.push_back("S035B57"); 
  nodetitle[inode]="G(h- h- h+ 3pi0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(204);       node_num_coef[inode].push_back(1.        );              
  ++inode;//22
  nodegammaname.push_back("Gamma152"); 
  nodename.push_back("S035B58"); 
  nodetitle[inode]="G(h- omega pi0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(1.        );              
  ++inode;//23
  nodegammaname.push_back("Gamma76"); 
  nodename.push_back("S035B59"); 
  nodetitle[inode]="G(h- h- h+ 2pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=3; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(8.8800E-01);              
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(2.2600E-01);               
  ++inode;//24
  nodegammaname.push_back("Gamma57"); 
  nodename.push_back("S035B62"); 
  nodetitle[inode]="G(h- h- h+ nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=4; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(259);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(1.7000E-02);               
  ++inode;//25
  nodegammaname.push_back("Gamma55"); 
  nodename.push_back("S035B63"); 
  nodetitle[inode]="G( h- h- h+ >=0 neutrals  nu(tau)  (ex. K(S)0 --> pi+ pi-)(``3-prong''))/G(total)";
  node_num_npar[inode]=12; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.8500E-01);              
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(9.1010E-01);               
  node_num_parm[inode].push_back(204);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(259);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(2.8500E-01);               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(9.1010E-01);               
  ++inode;//26
  nodegammaname.push_back("Gamma57by55"); 
  nodename.push_back("S035B64"); 
  nodetitle[inode]="G(h- h- h+ nu(tau) (ex.K0))/G( h- h- h+ >=0 neutrals  nu(tau)  (ex. K(S)0 --> pi+ pi-)(``3-prong''))";
  node_num_npar[inode]=4; node_den_npar[inode]=12;
  node_num_parm[inode].push_back(259);       node_num_coef[inode].push_back(1.        );            
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(1.7000E-02);               
  node_den_parm[inode].push_back(109);       node_den_coef[inode].push_back(2.8500E-01);               
  node_den_parm[inode].push_back(113);       node_den_coef[inode].push_back(9.1010E-01);               
  node_den_parm[inode].push_back(204);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(216);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(247);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(259);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(260);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(263);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(285);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(5  );       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(58 );       node_den_coef[inode].push_back(2.8500E-01);               
  node_den_parm[inode].push_back(8  );       node_den_coef[inode].push_back(9.1010E-01);               
  ++inode;//27
  nodegammaname.push_back("Gamma34"); 
  nodename.push_back("S035B67"); 
  nodetitle[inode]="G(h- Kbar0 nu(tau))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(1.        );               
  ++inode;//28
  nodegammaname.push_back("Gamma39"); 
  nodename.push_back("S035B68"); 
  nodetitle[inode]="G(h- Kbar0 pi0 nu(tau))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.        );               
  ++inode;//29
  nodegammaname.push_back("Gamma47"); 
  nodename.push_back("S035B69"); 
  nodetitle[inode]="G(pi- K(S)0 K(S)0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(1.        );              
  ++inode;//30
  nodegammaname.push_back("Gamma58"); 
  nodename.push_back("S035B71"); 
  nodetitle[inode]="G(h- h- h+ nu(tau) (ex.K0,omega))/G(total)";
  node_num_npar[inode]=3; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(259);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  ++inode;//31
  nodegammaname.push_back("Gamma77");
  nodename.push_back("S035B72"); 
  nodetitle[inode]="G(h- h- h+ 2pi0 nu(tau) (ex.K0,omega,eta))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );              
  ++inode;//32
  nodegammaname.push_back("Gamma8"); 
  nodename.push_back("S035B73"); 
  nodetitle[inode]="G(h- nu(tau))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(12 );       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(7  );       node_num_coef[inode].push_back(1.        );               
  ++inode;//33
  nodegammaname.push_back("Gamma18");
  nodename.push_back("S035B74"); 
  nodetitle[inode]="G(h- 2pi0 nu(tau))/G(total)";
  node_num_npar[inode]=4; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(201);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(1.5700E-01);               
  ++inode;//34
  nodegammaname.push_back("Gamma1");
  nodename.push_back("S035B75"); 
  nodetitle[inode]="G(particle->=0 neutrals >=0K0 nu(tau)  (``1-prong''))/G(total)";
  node_num_npar[inode]=21; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(1  );       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(7.1500E-01);               
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(9.0000E-02);               
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(12 );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(16 );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(182);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(2  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(201);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(2.0000E+00);               
  node_num_parm[inode].push_back(240);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(7.0800E-01);               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(7  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(9.0000E-02);               
  ++inode;//35
  nodegammaname.push_back("Gamma65");
  nodename.push_back("S035B76"); 
  nodetitle[inode]="G(h- h- h+ pi0 nu(tau))/G(total)";
  node_num_npar[inode]=8; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.2600E-01);              
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(1.7000E-02);               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(8.8800E-01);               
  ++inode;//36
  nodegammaname.push_back("Gamma75");
  nodename.push_back("S035B77");
  nodetitle[inode]="G(h- h- h+ 2pi0 nu(tau))/G(total)";
  node_num_npar[inode]=4; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(8.8800E-01);              
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(4.3070E-01);               
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(2.2600E-01);               
  ++inode;//37
  nodegammaname.push_back("Gamma64");
  nodename.push_back("S035B78"); 
  nodetitle[inode]="G(h- h- h+ >=1 pi0 nu(tau) (ex. K0))/G(total)";
  node_num_npar[inode]=9; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.2600E-01);              
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(9.1010E-01);               
  node_num_parm[inode].push_back(204);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(2.2600E-01);               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(8.8800E-01);               
  ++inode;//38
  nodegammaname.push_back("Gamma29"); 
  nodename.push_back("S035B79"); 
  nodetitle[inode]="G(h- 4pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(3.1900E-01);               
  ++inode;//39
  nodegammaname.push_back("Gamma8by5");
  nodename.push_back("S035B97"); 
  nodetitle[inode]="G(h- nu(tau))/G(e- nubar(e) nu(tau))";
  node_num_npar[inode]=2; node_den_npar[inode]=1;
  node_num_parm[inode].push_back(12 );       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(7  );       node_num_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(2  );       node_den_coef[inode].push_back(1.        );               
  ++inode;//40
  nodegammaname.push_back("Gamma12");
  nodename.push_back("S035C01"); 
  nodetitle[inode]="G(h- >= 1pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=9; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(3.2500E-01);              
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(16 );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(182);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(201);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(3.2500E-01);               
  ++inode;//41
  nodegammaname.push_back("Gamma25"); 
  nodename.push_back("S035C02"); 
  nodetitle[inode]="G(h- >= 3pi0 nu(tau) (ex. K0))/G(total)";
  node_num_npar[inode]=5; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(3.2500E-01);              
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(3.2500E-01);               
  ++inode;//42
  nodegammaname.push_back("Gamma74");
  nodename.push_back("S035C03");
  nodetitle[inode]="G(h- h- h+ >= 2pi0 nu(tau) (ex. K0))/G(total)";
  node_num_npar[inode]=4; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(8.8800E-01);              
  node_num_parm[inode].push_back(204);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(2.2600E-01);               
  ++inode;//43
  nodegammaname.push_back("Gamma48");
  nodename.push_back("S035C1");
  nodetitle[inode]="G(pi- K(S)0 K(L)0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(240);       node_num_coef[inode].push_back(1.        );              
  ++inode;//44
  nodegammaname.push_back("Gamma59");
  nodename.push_back("S035C18"); 
  nodetitle[inode]="G(pi- pi+ pi- nu(tau))/G(total)";
  node_num_npar[inode]=3; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(3.4310E-01);              
  node_num_parm[inode].push_back(259);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(1.7000E-02);               
  ++inode;//45
  nodegammaname.push_back("Gamma60");
  nodename.push_back("S035C19");
  nodetitle[inode]="G(pi- pi+ pi- nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(259);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(1.7000E-02);               
  ++inode;//46
  nodegammaname.push_back("Gamma62");
  nodename.push_back("S035C20");
  nodetitle[inode]="G(pi- pi+ pi- nu(tau) (ex.K0,omega))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(259);       node_num_coef[inode].push_back(1.        );              
  ++inode;//47
  nodegammaname.push_back("Gamma85");
  nodename.push_back("S035C21");
  nodetitle[inode]="G(K- pi+ pi- nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );              
  ++inode;//48
  nodegammaname.push_back("Gamma68");
  nodename.push_back("S035C22");
  nodetitle[inode]="G(pi- pi+ pi- pi0 nu(tau))/G(total)";
  node_num_npar[inode]=4; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(1.7000E-02);              
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(8.8800E-01);               
  ++inode;//49
  nodegammaname.push_back("Gamma69");
  nodename.push_back("S035C23");
  nodetitle[inode]="G(pi- pi+ pi- pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=3; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(1.7000E-02);              
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(8.8800E-01);               
  ++inode;//50
  nodegammaname.push_back("Gamma70");
  nodename.push_back("S035C24");
  nodetitle[inode]="G(pi- pi+ pi- pi0 nu(tau) (ex.K0,omega))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );              
  ++inode;//51
  nodegammaname.push_back("Gamma88"); 
  nodename.push_back("S035C25");
  nodetitle[inode]="G(K- pi+ pi- pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.2600E-01);              
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  ++inode;//52
  nodegammaname.push_back("Gamma80"); 
  nodename.push_back("S035C31"); 
  nodetitle[inode]="G(K- h+ pi- nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  ++inode;//53
  nodegammaname.push_back("Gamma80by60"); 
  nodename.push_back("S035C32"); 
  nodetitle[inode]="G(K- h+ pi- nu(tau) (ex.K0))/G(pi- pi+ pi- nu(tau) (ex.K0))";
  node_num_npar[inode]=2; node_den_npar[inode]=2;
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(259);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(8  );       node_den_coef[inode].push_back(1.7000E-02);               
  ++inode;//54
  nodegammaname.push_back("Gamma81"); 
  nodename.push_back("S035C33"); 
  nodetitle[inode]="G(K- h+ pi- pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=3; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.2600E-01);              
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  ++inode;//55
  nodegammaname.push_back("Gamma81by69");
  nodename.push_back("S035C34");
  nodetitle[inode]="G(K- h+ pi- pi0 nu(tau) (ex.K0))/G(pi- pi+ pi- pi0 nu(tau) (ex.K0))";
  node_num_npar[inode]=3; node_den_npar[inode]=3;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.2600E-01);              
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(113);       node_den_coef[inode].push_back(1.7000E-02);               
  node_den_parm[inode].push_back(263);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(8  );       node_den_coef[inode].push_back(8.8800E-01);               
  ++inode;//56
  nodegammaname.push_back("Gamma93by60");
  nodename.push_back("S035C35");
  nodetitle[inode]="G(K- K+ pi- nu(tau))/G(pi- pi+ pi- nu(tau) (ex.K0))";
  node_num_npar[inode]=1; node_den_npar[inode]=2;
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );              
  node_den_parm[inode].push_back(259);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(8  );       node_den_coef[inode].push_back(1.7000E-02);               
  ++inode;//57
  nodegammaname.push_back("Gamma94by69");
  nodename.push_back("S035C36");
  nodetitle[inode]="G(K- K+ pi- pi0 nu(tau))/G(pi- pi+ pi- pi0 nu(tau) (ex.K0))";
  node_num_npar[inode]=1; node_den_npar[inode]=3;
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );              
  node_den_parm[inode].push_back(113);       node_den_coef[inode].push_back(1.7000E-02);               
  node_den_parm[inode].push_back(263);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(8  );       node_den_coef[inode].push_back(8.8800E-01);               
  ++inode;//58
  nodegammaname.push_back("Gamma38");
  nodename.push_back("S035C38");
  nodetitle[inode]="G(K- K0 >=0pi0 nu(tau))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(1.        );               
  ++inode;//59
  nodegammaname.push_back("Gamma83");
  nodename.push_back("S035C40"); 
  nodetitle[inode]="G(K- pi+ pi- >=0pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=3; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.2600E-01);              
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  ++inode;//60
  nodegammaname.push_back("Gamma110");
  nodename.push_back("S035C47"); 
  nodetitle[inode]="G(X- (S=-1) nu(tau))/G(total)";
  node_num_npar[inode]=9; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(182);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(7  );       node_num_coef[inode].push_back(1.        );               
  ++inode;//61
  nodegammaname.push_back("Gamma89");
  nodename.push_back("S035C54"); 
  nodetitle[inode]="G(K- pi+ pi- pi0 nu(tau) (ex.K0,eta))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );              
  ++inode;//62
  nodegammaname.push_back("Gamma84");
  nodename.push_back("S035C6"); 
  nodetitle[inode]="G(K- pi+ pi- nu(tau))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(3.4310E-01);               
  ++inode;//63
  nodegammaname.push_back("Gamma87");
  nodename.push_back("S035C7");
  nodetitle[inode]="G(K- pi+ pi- pi0 nu(tau))/G(total)";
  node_num_npar[inode]=3; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.2600E-01);              
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  ++inode;//64
  nodegammaname.push_back("Gamma94"); 
  nodename.push_back("S035C8");
  nodetitle[inode]="G(K- K+ pi- pi0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );             
  ++inode;//65
  nodegammaname.push_back("Gamma3"); 
  nodename.push_back("S035R1");
  nodetitle[inode]="G(mu- nubar(mu) nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(1  );       node_num_coef[inode].push_back(1.        );              
  ++inode;//66
  nodegammaname.push_back("Gamma150by66"); 
  nodename.push_back("S035R14");
  nodetitle[inode]="G(h- omega nu(tau))/G(h- h- h+ pi0 nu(tau) (ex.K0))";
  node_num_npar[inode]=1; node_den_npar[inode]=6;
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(1.        );              
  node_den_parm[inode].push_back(109);       node_den_coef[inode].push_back(2.2600E-01);               
  node_den_parm[inode].push_back(113);       node_den_coef[inode].push_back(1.7000E-02);               
  node_den_parm[inode].push_back(247);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(263);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(285);       node_den_coef[inode].push_back(1.        );               
  node_den_parm[inode].push_back(8  );       node_den_coef[inode].push_back(8.8800E-01);               
  ++inode;//67
  nodegammaname.push_back("Gamma149"); 
  nodename.push_back("S035R15"); 
  nodetitle[inode]="G(h- omega  >=0 neutrals  nu(tau))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(1.        );               
  ++inode;//68
  nodegammaname.push_back("Gamma5"); 
  nodename.push_back("S035R2");
  nodetitle[inode]="G(e- nubar(e) nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(2  );       node_num_coef[inode].push_back(1.        );              
  ++inode;//69
  nodegammaname.push_back("Gamma19"); 
  nodename.push_back("S035R20");
  nodetitle[inode]="G(h- 2pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(201);       node_num_coef[inode].push_back(1.        );               
  ++inode;//70
  nodegammaname.push_back("Gamma26");
  nodename.push_back("S035R21");
  nodetitle[inode]="G(h- 3pi0 nu(tau))/G(total)";
  node_num_npar[inode]=5; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(3.2200E-01);              
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );               
  ++inode;//71
  nodegammaname.push_back("Gamma150");
  nodename.push_back("S035R23");
  nodetitle[inode]="G(h- omega nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(1.        );              
  ++inode;//72
  nodegammaname.push_back("Gamma2");
  nodename.push_back("S035R24");
  nodetitle[inode]="G(particle->=0 neutrals >=0K(L)0 nu(tau) )/G(total)";
  node_num_npar[inode]=21; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(1  );       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(7.1500E-01);               
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(9.0000E-02);               
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(6.5690E-01);               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(6.5690E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(6.5690E-01);               
  node_num_parm[inode].push_back(12 );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(16 );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(182);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(2  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(201);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(1.0985E+00);               
  node_num_parm[inode].push_back(240);       node_num_coef[inode].push_back(3.1390E-01);               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(7.0800E-01);               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(6.5690E-01);               
  node_num_parm[inode].push_back(7  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(9.0000E-02);               
  ++inode;//73
  nodegammaname.push_back("Gamma31");
  nodename.push_back("S035R26");
  nodetitle[inode]="G(K- >=0pi0 >=0K0  >=0gamma  nu(tau))/G(total)";
  node_num_npar[inode]=7; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(7.1500E-01);              
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(182);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(7  );       node_num_coef[inode].push_back(1.        );               
  ++inode;//74
  nodegammaname.push_back("Gamma32");
  nodename.push_back("S035R27");
  nodetitle[inode]="G(K- >=1 (pi0 or K0 or gamma)  nu(tau))/G(total)";
  node_num_npar[inode]=6; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(7.1500E-01);              
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(182);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(1.        );               
  ++inode;//75
  nodegammaname.push_back("Gamma56");
  nodename.push_back("S035R28");
  nodetitle[inode]="G(h- h- h+ nu(tau))/G(total)";
  node_num_npar[inode]=6; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(3.4310E-01);              
  node_num_parm[inode].push_back(259);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(1.7000E-02);               
  ++inode;//76
  nodegammaname.push_back("Gamma63");
  nodename.push_back("S035R30");
  nodetitle[inode]="G(h- h- h+ >=1 neutrals  nu(tau))/G(total)";
  node_num_npar[inode]=13; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.8500E-01);              
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(9.1010E-01);               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(204);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(4.3070E-01);               
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(240);       node_num_coef[inode].push_back(6.8610E-01);               
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(2.8500E-01);               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(8.8800E-01);               
  ++inode;//77
  nodegammaname.push_back("Gamma54");
  nodename.push_back("S035R31");
  nodetitle[inode]="G(h- h- h+ >=0 neutrals >=0K(L)0 nu(tau) )/G(total)";
  node_num_npar[inode]=18; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.8500E-01);              
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(9.1010E-01);               
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(204);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(4.3070E-01);               
  node_num_parm[inode].push_back(216);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(240);       node_num_coef[inode].push_back(6.8610E-01);               
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(259);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(263);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(2.8500E-01);               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(9.1010E-01);               
  ++inode;//78
  nodegammaname.push_back("Gamma126");
  nodename.push_back("S035R32");
  nodetitle[inode]="G(eta pi- pi0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(1.        );              
  ++inode;//79
  nodegammaname.push_back("Gamma102");
  nodename.push_back("S035R33");
  nodetitle[inode]="G( 3h- 2h+ >=0 neutrals  nu(tau)  (ex. K(S)0 --> pi- pi+)(``5-prong''))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(3  );       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(4  );       node_num_coef[inode].push_back(1.        );               
  ++inode;//80
  nodegammaname.push_back("Gamma79");
  nodename.push_back("S035R34");
  nodetitle[inode]="G(K- h+ h- >=0 neutrals  nu(tau))/G(total)";
  node_num_npar[inode]=7; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.8500E-01);              
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(247);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(3.4310E-01);               
  ++inode;//81
  nodegammaname.push_back("Gamma103");
  nodename.push_back("S035R38");
  nodetitle[inode]="G(3h- 2h+ nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(3  );       node_num_coef[inode].push_back(1.        );              
  ++inode;//82
  nodegammaname.push_back("Gamma104");
  nodename.push_back("S035R39");
  nodetitle[inode]="G(3h- 2h+ pi0 nu(tau) (ex.K0))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(4  );       node_num_coef[inode].push_back(1.        );              
  ++inode;//83
  nodegammaname.push_back("Gamma93");
  nodename.push_back("S035R40");
  nodetitle[inode]="G(K- K+ pi- nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(5  );       node_num_coef[inode].push_back(1.        );              
  ++inode;//84
  nodegammaname.push_back("Gamma82");
  nodename.push_back("S035R41");
  nodetitle[inode]="G(K- pi+ pi- >=0 neutrals  nu(tau))/G(total)";
  node_num_npar[inode]=5; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(2.8500E-01);              
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(3.4310E-01);               
  node_num_parm[inode].push_back(260);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(285);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(3.4310E-01);               
  ++inode;//85
  nodegammaname.push_back("Gamma11");
  nodename.push_back("S035R42");
  nodetitle[inode]="G(h- >=1 neutrals nu(tau))/G(total)";
  node_num_npar[inode]=16; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(7.1500E-01);              
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(113);       node_num_coef[inode].push_back(9.0000E-02);               
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(16 );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(182);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(201);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(9.8500E-02);               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(7.0800E-01);               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(8  );       node_num_coef[inode].push_back(9.0000E-02);               
  ++inode;//86
  nodegammaname.push_back("Gamma7");
  nodename.push_back("S035R43");
  nodetitle[inode]="G(h- >=0K(L)0  nu(tau))/G(total)";
  node_num_npar[inode]=5; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(5.0000E-01);              
  node_num_parm[inode].push_back(12 );       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(5.0000E-01);               
  node_num_parm[inode].push_back(7  );       node_num_coef[inode].push_back(1.        );               
  ++inode;//87
  nodegammaname.push_back("Gamma17");
  nodename.push_back("S035R44");
  nodetitle[inode]="G(h- >= 2pi0 nu(tau))/G(total)";
  node_num_npar[inode]=12; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(3.2200E-01);              
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(115);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(117);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(201);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(9.8500E-02);               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(3.1900E-01);               
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(1.5700E-01);               
  ++inode;//88
  nodegammaname.push_back("Gamma37");
  nodename.push_back("S035R46");
  nodetitle[inode]="G(K- K0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(62 );       node_num_coef[inode].push_back(1.        );              
  ++inode;//89
  nodegammaname.push_back("Gamma3by5");
  nodename.push_back("S035R5");
  nodetitle[inode]="G(mu- nubar(mu) nu(tau))/G(e- nubar(e) nu(tau))";
  node_num_npar[inode]=1; node_den_npar[inode]=1;
  node_num_parm[inode].push_back(1  );       node_num_coef[inode].push_back(1.        );              
  node_den_parm[inode].push_back(2  );       node_den_coef[inode].push_back(1.        );               
  ++inode;//90
  nodegammaname.push_back("Gamma9");
  nodename.push_back("S035R6");
  nodetitle[inode]="G(pi- nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(12 );       node_num_coef[inode].push_back(1.        );              
  ++inode;//91
  nodegammaname.push_back("Gamma10");
  nodename.push_back("S035R7");
  nodetitle[inode]="G(K- nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(7  );       node_num_coef[inode].push_back(1.        );              
  ++inode;//92
  nodegammaname.push_back("Gamma14");
  nodename.push_back("S035R8");
  nodetitle[inode]="G(pi- pi0 nu(tau))/G(total)";
  node_num_npar[inode]=1; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(16 );       node_num_coef[inode].push_back(1.        );              
  ++inode;//93
  nodegammaname.push_back("Gamma13"); 
  nodename.push_back("S035R84"); 
  nodetitle[inode]="G(h- pi0 nu(tau))/G(total)";
  node_num_npar[inode]=2; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(16 );       node_num_coef[inode].push_back(1.        );              
  node_num_parm[inode].push_back(182);       node_num_coef[inode].push_back(1.        );               
  ++inode;//94
  nodegammaname.push_back("Gamma24");
  nodename.push_back("S035R97");
  nodetitle[inode]="G(h- >= 3pi0 nu(tau))/G(total)";
  node_num_npar[inode]=8; node_den_npar[inode]=0;
  node_num_parm[inode].push_back(109);       node_num_coef[inode].push_back(3.2200E-01);              
  node_num_parm[inode].push_back(110);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(116);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(118);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(119);       node_num_coef[inode].push_back(1.5700E-01);               
  node_num_parm[inode].push_back(203);       node_num_coef[inode].push_back(1.        );               
  node_num_parm[inode].push_back(214);       node_num_coef[inode].push_back(9.8500E-02);               
  node_num_parm[inode].push_back(58 );       node_num_coef[inode].push_back(3.1900E-01);               
  ++inode;//95
  /* 
  //
  // PRINT NODE DEFINITION [NO NEED TO DO THIS EVERY TIME, SINCE IT DOES NOT CHANGE]
  //
  FILE *nodefile[2];
  nodefile[0]=fopen("all_node_def.txt","w");
  nodefile[1]=fopen("derived_node_def.txt","w");
  for (int p=0;p<2;++p) {
    for (inode=0;inode<nnode;++inode) {
      if (p==1&&(node_num_npar[inode]+node_den_npar[inode])==1) continue;
      fprintf (nodefile[p], "\n* %s : %s \n%s = ",nodename[inode].data(),nodetitle[inode].data(), nodegammaname[inode].data());
      for (ipar=0; ipar < node_num_parm[inode].size(); ++ipar) {
        if (ipar==0) { fprintf (nodefile[p], "(") ; } else {fprintf (nodefile[p], " + ") ;}
        int parm=node_num_parm[inode].at(ipar);
        vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
        int quan=ibase-baseparm.begin()+1;
        fprintf (nodefile[p], "%f*Gamma%d",node_num_coef[inode].at(ipar), basegamma[quan-1]);
        if (ipar==node_num_parm[inode].size()-1) fprintf (nodefile[p], ")");
      }
      if (node_den_parm[inode].size()==0) fprintf (nodefile[p], "\n") ; 
      for (ipar=0; ipar < node_den_parm[inode].size(); ++ipar) {
        if (ipar==0) { fprintf (nodefile[p], " / (") ; } else {fprintf (nodefile[p], " + ") ;}
        int parm=node_den_parm[inode].at(ipar);
        vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
        int quan=ibase-baseparm.begin()+1;
        fprintf (nodefile[p], "%f*Gamma%d",node_den_coef[inode].at(ipar), basegamma[quan-1]);
        if (ipar==node_den_parm[inode].size()-1) fprintf (nodefile[p], ")\n");
      }
    }
    fclose(nodefile[p]);
  }
  */
  //
  // Input R [node] as a linearized sum of variables P_i [base quantities]
  //
  // General Case:
  // R(P_i) = (sum_j=1,n alpha_j P_j) / (sum_k=1,m beta_k P_k)
  // dR/dP_i= (1/sum_k=1,m beta_i P_i) * alpha_j * delta_ij - (R/sum_k=1,m beta_k P_k) * beta_k * delta_ik
  //
  // Special Case 1:
  // R(P_i) = (sum_j=1,n alpha_j P_j) 
  // dR/dP_i= alpha_i 
  //
  // Special Case 2: 
  // R(P_i) = (sum_j=1,n alpha_j P_j) / (sum_k=1,m beta_k P_k)
  // Case 2a: if P_i only in numerator: dR/dP_i = (1/sum_k=1,m beta_k P_k) * alpha_i
  // Case 2b: if P_i only in denominator: dR/dP_i = - (R/sum_k=1,m beta_k P_k) * beta_i
  // Case 2c: if P_i both in numerator and denominator: dR/dP_i = (1/sum_k=1,m beta_i P_i) * alpha_i - (R/sum_k=1,m beta_k P_k) * beta_i
  //
  vector<int> node_parm[nnode]; // vector of parameters for each node
  vector<int> node_quan[nnode]; // vector of quantities for each node
  double node_num[nnode]; //   numerator sum [calculated using seed values of quantities] for each node
  double node_den[nnode]; // denominator sum [calculated using seed values of quantities] for each node
  vector<double> node_part[nnode]; // vector of partial derivatives w.r.t quantities for each node
  int first_quan[nnode];  // first quantity measured for each node
  for (inode=0;inode<nnode;++inode){
    node_parm[inode].insert(node_parm[inode].end(),node_num_parm[inode].begin(),node_num_parm[inode].end());
    node_parm[inode].insert(node_parm[inode].end(),node_den_parm[inode].begin(),node_den_parm[inode].end());
    sort(node_parm[inode].begin(), node_parm[inode].end());//sort must be called before unique
    vector<int>::iterator new_end=unique(node_parm[inode].begin(), node_parm[inode].end());
    node_parm[inode].erase(new_end, node_parm[inode].end()); // <--
    //
    double numerator, denominator, partial;
    //
    numerator=0; // sum_j=1,n alpha_j P_j
    if (node_num_npar[inode]>0) {
      for (ipar=0;ipar<node_num_npar[inode];++ipar){
	int parm=node_num_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	numerator+=(node_num_coef[inode].at(ipar))*(baseseed[quan-1]); 
      }
    }
    node_num[inode]=numerator; // <--
    //
    denominator=0; // sum_k=1,m beta_k P_k
    if (node_den_npar[inode]>0) {
      for (ipar=0;ipar<node_den_npar[inode];++ipar){
	int parm=node_den_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	denominator+=(node_den_coef[inode].at(ipar))*(baseseed[quan-1]); 
      }
    }
    node_den[inode]=denominator; // <--
    //
    first_quan[inode]=99; // <--
    for (ipar=0;ipar<node_parm[inode].size();++ipar){
      int parm=node_parm[inode].at(ipar);
      vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
      int quan=ibase-baseparm.begin()+1;
      node_quan[inode].push_back(quan); // <--
      if (quan<first_quan[inode]) first_quan[inode]=quan; // <--
      //
      vector<int>::iterator it_num=find(node_num_parm[inode].begin(),node_num_parm[inode].end(),parm); bool is_in_num = it_num != node_num_parm[inode].end();
      vector<int>::iterator it_den=find(node_den_parm[inode].begin(),node_den_parm[inode].end(),parm); bool is_in_den = it_den != node_den_parm[inode].end();
      partial=0;
      if (node_den_npar[inode]==0 && is_in_num) {//case 1
	partial = node_num_coef[inode].at(it_num - node_num_parm[inode].begin());
      } else if ( is_in_num && !is_in_den) { // case 2a
	partial = (1./denominator) * (node_num_coef[inode].at(it_num - node_num_parm[inode].begin()));
      } else if (!is_in_num &&  is_in_den) { // case 2b
	partial = -1. * (numerator/(denominator*denominator)) * (node_den_coef[inode].at(it_den - node_den_parm[inode].begin()));
      } else if ( is_in_num &&  is_in_den) { // case 2c
	partial = (1./denominator) * (node_num_coef[inode].at(it_num - node_num_parm[inode].begin())) -1. * (numerator/(denominator*denominator)) * (node_den_coef[inode].at(it_den - node_den_parm[inode].begin()));
      }
      node_part[inode].push_back(partial); // <--
      //      if (inode==14) cout << " inode = " << inode << " ipar = " << ipar << " parm = " << parm << " quan = " << quan << " partial = " << partial 
      //			  << " it_num - node_num_parm[inode].begin() = " << it_num - node_num_parm[inode].begin() 
      //			  << endl;
    }
  }
  //
  int nmeas=0;
  int measnum[200], imeas1, imeas2;
  string gammaname[200], measnodename[200], expname[200], author[200], year[200], meastitle[200];
  double measvalue[200], measerror[200], corrtemp, corrmat[200][200];
  for (imeas1=0;imeas1<200;imeas1++) for (imeas2=0;imeas2<200;imeas2++) corrmat[imeas1][imeas2] = 0;
  //
  ifstream ifs(Form("s035-fit-no-babar-belle%s.data",salephhcorr.data())) ;
  if (!ifs.good()) {cout << "Cannot open input file : " << Form("s035-fit-no-babar-belle%s.data",salephhcorr.data()) << endl ; exit(1) ;}
  while(ifs.good()) {
    if (ifs.eof()) break;
    char firstch(' ') ; ifs.get(firstch) ;
    if (firstch=='#'||firstch=='\n') { // Skip such lines
      ifs.ignore(256,'\n') ;
    } else if (firstch=='*') {  // measurement line
      ifs >> measnum[nmeas] >> gammaname[nmeas] >> measnodename[nmeas]
	  >> measvalue[nmeas] >> measerror[nmeas] 
	  >> expname[nmeas] >> author[nmeas] >> year[nmeas];
      ifs.getline(buffer,256,'\n');
      string stringbuffer=string(buffer);
      meastitle[nmeas]="";
      bool first=false;
      for (string::iterator si=stringbuffer.begin();si!=stringbuffer.end();++si){
	if (13 == (int)*si) break; // ^M is 13
	if (' ' != *si) first=true;
	if (first) meastitle[nmeas]+=*si;
      }
      //      cout << measnum[nmeas] << " " << gammaname[nmeas] << " " << measnodename[nmeas] << " " 
      //	   << measvalue[nmeas] << " " << measerror[nmeas] << " " 
      //	   << expname[nmeas] << " " << author[nmeas] << " " << year[nmeas] << " " << meastitle[nmeas] << endl;
      ++nmeas;
    } else if (firstch=='%') {  // correlation line
      ifs >> imeas1 >> imeas2 >> corrtemp;       ifs.ignore(256,'\n') ;
      if (corrmat[imeas1-1][imeas2-1] != 0) {cout << "WATCH OUT" << endl; exit(1);}
      corrmat[imeas1-1][imeas2-1] = corrmat[imeas2-1][imeas1-1] = corrtemp*1e-2;
      //      cout << "Correlation between imeas1 = " << measnum[imeas1-1] << " and imeas2 = " << measnum[imeas2-1] << " is = " << corrmat[imeas1-1][imeas2-1] << endl;
    }
  }
  //
  // PRINT INFORMATION ABOUT INPUT MEASUREMENTS
  //
  FILE *measinfofile=fopen(Form("s035-fit-no-babar-belle%s.info",salephhcorr.data()),"w");
  //
  fprintf (measinfofile, "nmeas = %d \n", nmeas);
  //
  vector<int> basequan_used_in_measured_basenodes;
  vector<int> basequan_used_in_measured_derivednodes;
  for (int i=0;i<nmeas;++i){
    vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
    inode=it-nodename.begin();
    if ((node_num_npar[inode]+node_den_npar[inode])>1) { // measurement of a derived node
      basequan_used_in_measured_derivednodes.insert(basequan_used_in_measured_derivednodes.end(),node_quan[inode].begin(),node_quan[inode].end());
      sort(basequan_used_in_measured_derivednodes.begin(),basequan_used_in_measured_derivednodes.end());
      vector<int>::iterator new_end=unique(basequan_used_in_measured_derivednodes.begin(),basequan_used_in_measured_derivednodes.end());
      basequan_used_in_measured_derivednodes.erase(new_end,basequan_used_in_measured_derivednodes.end());
    }else{ //measurement of a base quantity
      basequan_used_in_measured_basenodes.insert(basequan_used_in_measured_basenodes.end(),node_quan[inode].begin(),node_quan[inode].end());
      sort(basequan_used_in_measured_basenodes.begin(),basequan_used_in_measured_basenodes.end());
      vector<int>::iterator new_end=unique(basequan_used_in_measured_basenodes.begin(),basequan_used_in_measured_basenodes.end());
      basequan_used_in_measured_basenodes.erase(new_end,basequan_used_in_measured_basenodes.end());
    }
  }
  fprintf (measinfofile, "basequan_used_in_measured_basenodes.size() = %d ", basequan_used_in_measured_basenodes.size());
  fprintf (measinfofile, "[IF THIS IS NOT = %d, IT MEANS SOME BASE QUANTITIES ARE NOT MEASURED DIRECTLY BUT ONLY VIA MEASUREMENTS OF DERIVED NODES]\n", nbase); 
  for (ibase=0;ibase<nbase;++ibase) {
    vector<int>::iterator it=find(basequan_used_in_measured_basenodes.begin(),basequan_used_in_measured_basenodes.end(),ibase+1); 
    bool not_present = it == basequan_used_in_measured_basenodes.end();
    if (not_present) fprintf (measinfofile, "BASE = %d GAMMA = %d PARM = %d NODE = %s TITLE = %s not measured directly \n",
			      ibase+1, basegamma[ibase], baseparm[ibase], basenode[ibase].data(), basetitle[ibase].data());
  }
  fprintf (measinfofile, "\n");
  fprintf (measinfofile, "basequan_used_in_measured_derivednodes.size() = %d \n", basequan_used_in_measured_derivednodes.size());
  for (ibase=0;ibase<basequan_used_in_measured_derivednodes.size();++ibase) {
    int quan = basequan_used_in_measured_derivednodes.at(ibase) ;
    fprintf (measinfofile, "basetitle = %s with quan = %d gamma = %d appears in ",basetitle[quan-1].data(), quan, basegamma[quan-1]);
    int iquan_occurance=0;
    int inode_occurance=0;
    vector<int> vector_measnodes;
    for (int i=0;i<nmeas;++i){
      vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
      inode=it-nodename.begin();
      vector<int>::iterator itt=find(vector_measnodes.begin(),vector_measnodes.end(),inode);
      bool is_newnode = itt == vector_measnodes.end();
      for (int ii=0;ii<node_quan[inode].size();++ii){
	if (quan == node_quan[inode].at(ii)) { 
	  ++iquan_occurance;
	  if (is_newnode) {
	    ++inode_occurance;
	  }
	}
      }
      vector_measnodes.push_back(inode);
    }
    fprintf (measinfofile, "%d measurements of %d nodes. \nThese %d measurements are :\n", iquan_occurance, inode_occurance, iquan_occurance);
    iquan_occurance = 0;
    vector<int> vector_of_measurements_correlated_with_other_measurements;
    for (int i=0;i<nmeas;++i){
      bool meas_with_correlation = false;
      vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
      inode=it-nodename.begin();
      for (int ii=0;ii<node_quan[inode].size();++ii){
	if (quan == node_quan[inode].at(ii)) { 
	  ++iquan_occurance;
	  fprintf (measinfofile, " (%d) IMEAS = %d NODE = %d NAME = %s GAMMA = %s TITLE = %s EXP = %s\n",
		   iquan_occurance, i+1, inode, nodename[inode].data(), gammaname[i].data(), meastitle[i].data(), expname[i].data());
	  for (int j=0;j<nmeas;++j){
	    if (corrmat[i][j] != 0) {
	      meas_with_correlation = true;
	      break;
	    }
	  }
	}
      }
      if (meas_with_correlation) vector_of_measurements_correlated_with_other_measurements.push_back(i+1); // <- NOTE: +1
    }
    if (vector_of_measurements_correlated_with_other_measurements.size()!=0) {
      fprintf (measinfofile, "vector_of_measurements_correlated_with_other_measurements has size = %d : ",vector_of_measurements_correlated_with_other_measurements.size());
      for (int i=0; i < vector_of_measurements_correlated_with_other_measurements.size(); ++i) {
	iimeas = vector_of_measurements_correlated_with_other_measurements[i] -1; // <- NOTE: -1
	fprintf (measinfofile, "%d(%s) ",iimeas+1, expname[iimeas].data()); 
      }
      fprintf (measinfofile, "\n");
    }else{
      fprintf (measinfofile, "vector_of_measurements_correlated_with_other_measurements has size = 0\n");
    }
    fprintf (measinfofile, "\n");
  }
  //
  TMatrixD MeasErrorMatrix(nmeas,nmeas);
  for (i=0;i<nmeas;++i) {
    for (j=0;j<nmeas;++j) {
      if (i==j) {
	MeasErrorMatrix(i,j)=measerror[i]*measerror[i];
      }else{
	MeasErrorMatrix(i,j)=corrmat[i][j]*measerror[i]*measerror[j]; // by construction diagonal elements of corrmat has not been filled
      }
    }
  }
  Double_t determinant;
  TMatrixD InvMeasErrorMatrix = MeasErrorMatrix;
  InvMeasErrorMatrix.Invert(&determinant);  
  fprintf (measinfofile,"Determinant of MeasErrorMatrix = %10.5g\n", determinant);
  //
  vector<int> icorrj[200]; // vector of measurements correlated with this measurement
  for (i=0;i<nmeas;++i){
    for (j=0;j<nmeas;++j){
      if (InvMeasErrorMatrix[i][j]!=0.0) {
	icorrj[i].push_back(j);
      }
    }
  }
  //
  int ncorrij=0; // number of cycles of correlated measurements
  int ifirstj[200]; // first measurement in this cycle
  for (i=0;i<nmeas;++i) {
    if (icorrj[i].size() > 1) {
      bool isnew=true;
      for (j=0;j<i;++j){
	if (icorrj[j].size() > 1) {
	  if (find(icorrj[j].begin(),icorrj[j].end(),i)!=icorrj[j].end()) {
	    isnew=false;
	    break;
	  }
	}
      }
      if (isnew) {
	ifirstj[ncorrij] = i;
	++ncorrij;
      }
      //      cout << "i,isnew,ncorrij = " << i << " " << isnew << " " << ncorrij << endl;
    }
  }
  fprintf (measinfofile, "ncorrij = %d \n",ncorrij);
  vector<int> veccorrij[200]; // vector of correlated measurements per cycle
  for (i=0;i<ncorrij;++i){
    fprintf (measinfofile, "i = %d ifirstj[i]+1 = %3d ",i,ifirstj[i]+1);
    veccorrij[i].insert(veccorrij[i].end(),icorrj[ifirstj[i]].begin(),icorrj[ifirstj[i]].end());
    fprintf (measinfofile, "veccorrij[i].size() = %2d veccorrij[i][j]+1 = ",veccorrij[i].size());
    for (j=0;j<veccorrij[i].size();++j) fprintf (measinfofile, "%d ",veccorrij[i][j]+1);
    fprintf (measinfofile, "GammaName = "); 
    for (j=0;j<veccorrij[i].size();++j) fprintf (measinfofile, "%s ",gammaname[veccorrij[i][j]].data());
    fprintf (measinfofile,"\n");
  }
  //
  int newnode_all=-1;
  vector<int> vector_measnodes_all;
  int nodegroup_all[200]; // mapping of each measurement to groups of nodes
  int ncycle[200]; // mapping of each measurement to the cycle number
  for (i=0;i<nmeas;++i) {
    if (icorrj[i].size()>1) continue;
    ncycle[i] = -1; // cycle number for uncorrelated measurements
    vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
    inode=it-nodename.begin();
    vector<int>::iterator itt=find(vector_measnodes_all.begin(),vector_measnodes_all.end(),inode);
    bool is_newnode_all = itt == vector_measnodes_all.end();
    if (is_newnode_all) {
      ++newnode_all;
    }
    nodegroup_all[i] = newnode_all; // node-group number for uncorrelated measurements
    vector_measnodes_all.push_back(inode);
  }
  for (i=0;i<ncorrij;++i) {
    ++newnode_all;
    for (j=0;j<veccorrij[i].size();++j) {
      int meas=veccorrij[i][j];
      ncycle[meas] = i; // cycle number for each correlated measurement
      nodegroup_all[meas] = newnode_all; // node-group number for correlated measurements
    }
  }
  int n_nodegroup[200];
  for (inode=0;inode<=newnode_all;++inode){
    n_nodegroup[inode]=0;
    for (i=0;i<nmeas;++i){
      if (nodegroup_all[i]==inode){
	n_nodegroup[inode]+=1;
      }
    }
  }
  int ngroup[200];
  for (inode=0;inode<=newnode_all;++inode){
    for (i=0;i<nmeas;++i){
      if (nodegroup_all[i]==inode) {
	ngroup[i] = n_nodegroup[nodegroup_all[i]];
	fprintf (measinfofile, "i+1 = %3d nodegroup = %2d ngroup = %2d icorrj = %2d ncycle = %2d meas = %10.5e +- %10.5e %6s %s \n",
		 i+1,nodegroup_all[i],ngroup[i],icorrj[i].size(), ncycle[i], measvalue[i], measerror[i], expname[i].data(), meastitle[i].data());
      }
    }
  }
  fclose(measinfofile);
  //
  // WRITE OUT MEASUREMENT FILE
  //
  FILE *measfile[2];
  for (int p=0;p<2;++p){
    if (p==0) measfile[0]=fopen(Form("combos_measurements_%s%s.input",sconstrain.data(),salephhcorr.data()),"w");
    if (p==1) measfile[1]=fopen(Form("alucomb_measurements_%s%s.input",sconstrain.data(),salephhcorr.data()),"w");
    FILE *thisfile = measfile[p];
    iimeas=0;
    for (int i=0;i<nmeas;++i){
      ++iimeas;
      //      cout << "i = " << i << endl;
      fprintf (thisfile, "* IMEAS = %d \n",     iimeas);
      fprintf (thisfile, "* GAMMANAME = %s \n", gammaname[i].data());
      fprintf (thisfile, "* DECAYNAME = %s \n", meastitle[i].data());
      vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
      inode=it-nodename.begin();
      fprintf (thisfile, "* NODENAME = %s found at inode+1 = %d with NODENAME[inode] = %s has %d, %d, %d base quantities in numerator, denominator and both [excluding overlap] \n", measnodename[i].data(), inode+1, nodename[inode].data(),node_num_npar[inode], node_den_npar[inode],node_parm[inode].size());
      for (ipar=0;ipar<node_num_parm[inode].size();++ipar){
	int parm=node_num_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	fprintf (thisfile,"*                numerator of inode+1 = %d has gamma = %d parm = %d quan = %d title = %s seed = %f coef = %f\n",inode+1, basegamma[quan-1], parm, quan, basetitle[quan-1].data(), baseseed[quan-1], node_num_coef[inode].at(ipar));
      }
      for (ipar=0;ipar<node_den_parm[inode].size();++ipar){
	int parm=node_den_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	fprintf (thisfile,"*              denominator of inode+1 = %d has gamma = %d parm = %d quan = %d title = %s seed = %f coef = %f\n",inode+1, basegamma[quan-1], parm, quan, basetitle[quan-1].data(), baseseed[quan-1], node_den_coef[inode].at(ipar));
      }
      fprintf (thisfile, "*  first quantity measured by inode+1 = %d has gamma = %d parm = %d quan = %d title = %s\n",inode+1,basegamma[first_quan[inode]-1],baseparm[first_quan[inode]-1],first_quan[inode],basetitle[first_quan[inode]-1].data());
      //
      fprintf (thisfile, "\nBEGIN %s Gamma%s pub.%s.%s \n\n", expname[i].data(), gammaname[i].data(), author[i].data(), year[i].data());
      if (p==0) {//COMBOS
	if (uconstrain &&      // SPECIAL CASE [because these nodes contain Gamma103 [used to express unitarity constraint]
	    ((inode+1)==80 ||  // NODE = 79 NAME = S035R33 GAMMA = 102, because Gamma102 = (1.000000*Gamma103 + 1.000000*Gamma104) 
	     (inode+1)==82)) { // NODE = 81 NAME = S035R38 GAMMA = 103
	  fprintf (thisfile, "MEASUREMENT  m_Gamma%d statistical systematic \n",3);
	  fprintf (thisfile, "DATA         m_Gamma%d statistical systematic \n",3);
	}else{
	  fprintf (thisfile, "MEASUREMENT  m_Gamma%d statistical systematic \n",basegamma[first_quan[inode]-1]);
	  fprintf (thisfile, "DATA         m_Gamma%d statistical systematic \n",basegamma[first_quan[inode]-1]);
	}
      }else if (p==1) {//ALUCOMB
	fprintf (thisfile, "MEASUREMENT  m_Gamma%s statistical systematic \n",gammaname[i].data());
	fprintf (thisfile, "DATA         m_Gamma%s statistical systematic \n",gammaname[i].data());
      }
      fprintf (thisfile, "             %10.5g %10.5g  0 \n",measvalue[i],measerror[i]);
      bool firstcorr=true;
      jjmeas=0;
      for (int j=0;j<nmeas;++j) {
	++jjmeas;
	if (corrmat[i][j]!=0) {
	  if (firstcorr) {fprintf (thisfile, " \n"); firstcorr=false;}
	  fprintf (thisfile, "STAT_CORR_WITH %s Gamma%s pub.%s.%s %f ! IMEAS = %d \n", 
		   expname[j].data(), gammaname[j].data(), author[j].data(), year[j].data(), corrmat[i][j], jjmeas);
	}
      }
      fprintf (thisfile, " \nEND \n\n");
    }
    fclose(thisfile);
  }
  //
  FILE *avefile[2];
  for (int p=0;p<2;++p){
    if (p==0) avefile[0]=fopen(Form("combos_average_no_babar_belle_%s%s.input",sconstrain.data(),salephhcorr.data()),"w");
    if (p==1) avefile[1]=fopen(Form("alucomb_average_no_babar_belle_%s%s.input",sconstrain.data(),salephhcorr.data()),"w");
    if (p==0) fprintf (avefile[p], "INCLUDE combos_measurements_%s%s.input \n\n",sconstrain.data(),salephhcorr.data()); 
    if (p==1) fprintf (avefile[p], "INCLUDE alucomb_measurements_%s%s.input \n\n",sconstrain.data(),salephhcorr.data()); 
    fprintf (avefile[p], "BEGIN   PDG-BABAR-BELLE all_methods \n\n");
    fprintf (avefile[p], "COMBINE * * * \n\n");
    for (ibase=0;ibase<nbase;++ibase){
      if (p==0&&uconstrain&&ibase==(nbase-1)){/* skip */}else{
	fprintf (avefile[p], "MEASUREMENT m_Gamma%d statistical systematic   ! NQUAN = %d \n",basegamma.at(ibase),ibase+1);  
      }
    }
    if (p==0){
      fprintf (avefile[p], "\nCALL DUMP_MASTER_INC \n\n");
      fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_PRT 1.0 0. \n\n");
      fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_INV 1 0    \n\n");
    }
    //
    vector<int> vector_measnodes;
    int usum=0;//number of [unique] measurements to be expressed as linearized sum of base quantities
    for (i=0;i<nmeas;++i){
      vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
      inode=it-nodename.begin();
      vector<int>::iterator itt=find(vector_measnodes.begin(),vector_measnodes.end(),inode);
      bool is_newnode = itt == vector_measnodes.end();
      if ((node_num_npar[inode]+node_den_npar[inode])>1) { // derived node
	if (p==1&&is_newnode){//new node
	  ++usum;
	  fprintf (avefile[p], "MEASUREMENT m_Gamma%s statistical systematic   ! NQUAN = %d \n",gammaname[i].data(),ibase+usum);  
	}
      }
      vector_measnodes.push_back(inode);      
    }
    //
    if (p==1&&!uconstrain){
      fprintf (avefile[p], "*--- dummy mode to remove unitarity constraint\nMEASUREMENT m_Gamma998 statistical systematic\n");
    }
    if (p==1) {
      fprintf (avefile[p], "*--- Gamma110, only present in constraint\nMEASUREMENT m_Gamma110 statistical systematic\n");
    }
    //
    int isum=0;//number of          measurements to be expressed as linearized sum of base quantities
    vector_measnodes.erase(vector_measnodes.begin(),vector_measnodes.end());
    for (int i=0;i<nmeas;++i){
      vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
      inode=it-nodename.begin();
      vector<int>::iterator itt=find(vector_measnodes.begin(),vector_measnodes.end(),inode);
      bool is_newnode = itt == vector_measnodes.end();
      if (p==1 && !is_newnode) continue; // ALUCOMB needs it only once
      if ((node_num_npar[inode]+node_den_npar[inode])>1) {
	//
	if (p==0 && ( (uconstrain) && ( (inode+1)==80) || ((inode+1)==82) ) ) continue; // SPECIAL CASE [because these are derived nodes containing Gamma103 ]
	++isum; // translate C index to Fortran index
	//
	// PRINT NODE DEFINITION
	//
	fprintf (avefile[p], "\n* Gamma%s = ",gammaname[i].data());
	for (ipar=0; ipar < node_num_parm[inode].size(); ++ipar) {
	  if (ipar==0) { fprintf (avefile[p], "(") ; } else {fprintf (avefile[p], " + ") ;}
	  int parm=node_num_parm[inode].at(ipar);
	  vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	  int quan=ibase-baseparm.begin()+1;
	  fprintf (avefile[p], "%f*Gamma%d",node_num_coef[inode].at(ipar), basegamma[quan-1]);
	  if (ipar==node_num_parm[inode].size()-1) fprintf (avefile[p], ")");
	}
	if (node_den_parm[inode].size()==0) fprintf (avefile[p], "\n") ; 
	for (ipar=0; ipar < node_den_parm[inode].size(); ++ipar) {
	  if (ipar==0) { fprintf (avefile[p], " / (") ; } else {fprintf (avefile[p], " + ") ;}
	  int parm=node_den_parm[inode].at(ipar);
	  vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	  int quan=ibase-baseparm.begin()+1;
	  fprintf (avefile[p], "%f*Gamma%d",node_den_coef[inode].at(ipar), basegamma[quan-1]);
	  if (ipar==node_den_parm[inode].size()-1) fprintf (avefile[p], ")\n");
	}
	//
	double offset = -node_num[inode]; // - [ f(x0,y0) - df/dx(x=x0) x0 - df/dy(y=y0) y0 - ...]
	if (node_den_npar[inode]>0) offset /= node_den[inode];
	//	  cout << inode << " " << node_num[inode] << " " << node_den[inode] << " " << offset << " " << measvalue[i] << endl;
	for (ipar = 0; ipar < node_parm[inode].size(); ++ipar) {
	  int quan=node_quan[inode].at(ipar);
	  double partial=node_part[inode].at(ipar);
	  offset += partial*baseseed[quan-1]; 
	}
	if (p==0) { // COMBOS
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d    %2d %d\n",isum,i+1,node_parm[inode].size()); 
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_AD %f 1.0\n",isum,offset); 
	} else if (p==1) { // ALUCOMB
	  fprintf (avefile[p], "CONSTRAINT Gamma%s.c %f Gamma%s -1", gammaname[i].data(), offset, gammaname[i].data());
	}
	for (ipar = 0; ipar < node_parm[inode].size(); ++ipar) {
	  int quan=node_quan[inode].at(ipar);
	  double partial=node_part[inode].at(ipar);
	  if (p==0) { // COMBOS
	    fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_%2.2d %2d %f ! Gamma%d \n",isum,ipar+1,quan,partial,basegamma.at(quan-1));
	  }else if (p==1) { // ALUCOMB
	    fprintf(avefile[p], " Gamma%d %f", basegamma[quan-1], partial);
	  }
	}
	if (p==1) fprintf(avefile[p], "\n");
      }
      vector_measnodes.push_back(inode);      
    }
    if (p==0&&uconstrain) {
      for (int i=0;i<nmeas;++i){
	vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
	inode=it-nodename.begin();
	//
	if ((inode+1)==80) { // SPECIAL CASE : NODE = 79 NAME = S035R33 GAMMA = 102 :: Gamma102 = (1.000000*Gamma103 + 1.000000*Gamma104)
	  fprintf(avefile[p], "\n*Gamma102 = 1 - Gamma3   - Gamma5   - Gamma9   - Gamma10  - Gamma14  - Gamma16\n");
	  fprintf(avefile[p], "*             - Gamma20  - Gamma23  - Gamma27  - Gamma28  - Gamma30 - Gamma35\n");
	  fprintf(avefile[p], "*             - Gamma37 - Gamma40  - Gamma42  - Gamma47  - Gamma48  - Gamma62\n");
	  fprintf(avefile[p], "*             - Gamma70 - Gamma77  - Gamma78  - Gamma85  - Gamma89  - Gamma93\n");
	  fprintf(avefile[p], "*             - Gamma94 - Gamma126 - Gamma128 - Gamma150 - Gamma152\n");
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d    %2d 29 \n",++isum,i+1); 
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_AD -1 +1 \n",isum); // becomes a measurement of -1+Gamma102; thats why the coefficients below have - sign 
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_01  1 -1 ! Gamma3  \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_02  2 -1 ! Gamma5  \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_03  3 -1 ! Gamma9  \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_04  4 -1 ! Gamma10 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_05  5 -1 ! Gamma14 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_06  6 -1 ! Gamma16 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_07  7 -1 ! Gamma20 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_08  8 -1 ! Gamma23 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_09  9 -1 ! Gamma27 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_10 10 -1 ! Gamma28 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_11 11 -1 ! Gamma30 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_12 12 -1 ! Gamma35 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_13 13 -1 ! Gamma37 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_14 14 -1 ! Gamma40 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_15 15 -1 ! Gamma42 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_16 16 -1 ! Gamma47 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_17 17 -1 ! Gamma48 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_18 18 -1 ! Gamma62 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_19 19 -1 ! Gamma70 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_20 20 -1 ! Gamma77 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_21 21 -1 ! Gamma78 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_22 22 -1 ! Gamma85 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_23 23 -1 ! Gamma89 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_24 24 -1 ! Gamma93 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_25 25 -1 ! Gamma94 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_26 27 -1 ! Gamma126\n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_27 28 -1 ! Gamma128\n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_28 29 -1 ! Gamma150\n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_29 30 -1 ! Gamma152\n",isum);
	}
	//
	if ((inode+1)==82) { // SPECIAL CASE : NODE = 81 NAME = S035R38 GAMMA = 103 
	  fprintf(avefile[p], "\n*Gamma103 = 1 - Gamma3   - Gamma5   - Gamma9   - Gamma10  - Gamma14  - Gamma16\n");
	  fprintf(avefile[p], "*             - Gamma20  - Gamma23  - Gamma27  - Gamma28  - Gamma30 - Gamma35\n");
	  fprintf(avefile[p], "*             - Gamma37 - Gamma40  - Gamma42  - Gamma47  - Gamma48  - Gamma62\n");
	  fprintf(avefile[p], "*             - Gamma70 - Gamma77  - Gamma78  - Gamma85  - Gamma89  - Gamma93\n");
	  fprintf(avefile[p], "*             - Gamma94 - Gamma104 - Gamma126 - Gamma128 - Gamma150 - Gamma152\n");
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d    %2d 30 \n",++isum,i+1); 
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_AD -1 +1 \n",isum); // becomes a measurement of -1+Gamma103; thats why the coefficients below have - sign 
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_01  1 -1 ! Gamma3  \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_02  2 -1 ! Gamma5  \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_03  3 -1 ! Gamma9  \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_04  4 -1 ! Gamma10 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_05  5 -1 ! Gamma14 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_06  6 -1 ! Gamma16 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_07  7 -1 ! Gamma20 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_08  8 -1 ! Gamma23 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_09  9 -1 ! Gamma27 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_10 10 -1 ! Gamma28 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_11 11 -1 ! Gamma30 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_12 12 -1 ! Gamma35 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_13 13 -1 ! Gamma37 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_14 14 -1 ! Gamma40 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_15 15 -1 ! Gamma42 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_16 16 -1 ! Gamma47 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_17 17 -1 ! Gamma48 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_18 18 -1 ! Gamma62 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_19 19 -1 ! Gamma70 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_20 20 -1 ! Gamma77 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_21 21 -1 ! Gamma78 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_22 22 -1 ! Gamma85 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_23 23 -1 ! Gamma89 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_24 24 -1 ! Gamma93 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_25 25 -1 ! Gamma94 \n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_26 26 -1 ! Gamma104\n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_27 27 -1 ! Gamma126\n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_28 28 -1 ! Gamma128\n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_29 29 -1 ! Gamma150\n",isum);
	  fprintf (avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_30 30 -1 ! Gamma152\n",isum);
	}
	//
      }
      if (p==0) fprintf (avefile[p], "\nSPARAMETER CHI2_N_SYM_NSUM  %d 0 \n",isum); 
    }
    if (p==1) {
      if (uconstrain) {
	fprintf(avefile[p], "\n* unitarity constraint applied (sum of basic modes without dummy mode)\n");
      }else{
	fprintf(avefile[p], "\n* unitarity constraint NOT applied (sum of basic modes with dummy mode)\n");
      }
      fprintf(avefile[p], "CONSTRAINT GammaAll 1\n");
      fprintf(avefile[p], "  Gamma3   1 Gamma5   1 Gamma9   1 Gamma10  1 Gamma14  1 Gamma16  1\n");
      fprintf(avefile[p], "  Gamma20  1 Gamma23  1 Gamma27  1 Gamma28  1 Gamma30  1 Gamma35  1\n");
      fprintf(avefile[p], "  Gamma37  1 Gamma40  1 Gamma42  1 Gamma47  1 Gamma48  1 Gamma62  1\n");
      fprintf(avefile[p], "  Gamma70  1 Gamma77  1 Gamma78  1 Gamma85  1 Gamma89  1 Gamma93  1\n");
      fprintf(avefile[p], "  Gamma94  1 Gamma103 1 Gamma104 1 Gamma126 1 Gamma128 1 Gamma150 1 Gamma152 1\n");
      if (!uconstrain) fprintf(avefile[p], "  Gamma998 1\n");
    }
    fprintf(avefile[p], "\nCALL CHI2_N_SYM\n");
    fprintf(avefile[p], "\nEND\n");
    fclose(avefile[p]);
  }
  //
  // measuredquantity = f(wxyz) = f(0) + f'w (w-w0) + f'x (x-x0) + f'y (y-y0) + f'z (z-z0)
  //                            = [f(0) - f'w w0 - f'x x0 - f'y y0 - f'z z0] + f'w w + f'x x + f'y y + f'z z

  // [measuredvalue - measuredquantity] = [measuredvalue - (f(0) - f'w w0 - f'x x0 - f'y y0 - f'z z0) - (f'w w + f'x x + f'y y + f'z z) ] 
  //                                    = Xvector + Delta^T Vvector
  // Xvector = vector [dimension = nmeas] of adjusted measured value. In this case, adjust measurement = M to M - f(0) + f'w w0 + f'x x0 + f'y y0 + f'z z0
  // Delta^T = matrix of dimension (nmeas,nbase). In this case it has dimension 1x4 = (-f'w -f'x -f'y -f'z)
  // Vvector = vector [dimension = nbase] of base quantity. In this case Vvector^T is 1x4 = (w,x,y,z)
  //
  TMatrixD Xvector(nmeas,1);
  TMatrixD Delta(nbase,nmeas);
  for (i=0;i<nmeas;++i){
    vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
    inode=it-nodename.begin();
    Xvector[i][0] = measvalue[i];
    double offset = -node_num[inode]; if (node_den_npar[inode]>0) offset /= node_den[inode];
    if ((node_num_npar[inode]+node_den_npar[inode])>1)  Xvector[i][0]+=offset;
    //    cout << i << " inode = " << inode << " expname = " << expname[i].data() << " meastitle = " << meastitle[i].data() << " measvalue = " << measvalue[i] 
    //	 << " offset = " << offset << " Xvector = " << Xvector[i][0] << endl;
    for (ipar = 0; ipar < node_parm[inode].size(); ++ipar) {
      int quan=node_quan[inode].at(ipar);
      double partial=node_part[inode].at(ipar);
      if ((node_num_npar[inode]+node_den_npar[inode])>1) Xvector[i][0] += partial*baseseed[quan-1]; 
      Delta[quan-1][i] = -partial;
      //      cout << "ipar = " << ipar << " quan = " << quan << " partial = " << partial << " baseseed = " << baseseed[quan-1] 
      //	   << " Xvector = " << Xvector[i][0] << " Delta = " << Delta[quan-1][i] << endl;
    }
  }
  //  TMatrixD DeltaT(nmeas,nbase);  DeltaT.Transpose(Delta);
  TMatrixD DeltaT(TMatrixD::kTransposed,Delta);
  TMatrixD SDInvMatrix = Delta * InvMeasErrorMatrix * DeltaT;
  Double_t sd_det;
  TMatrixD SDMatrix = SDInvMatrix;
  SDMatrix.Invert(&sd_det);
  TMatrixD Vvector = SDMatrix * Delta * InvMeasErrorMatrix * Xvector; Vvector*=-1;
  TMatrixD XPlusDTV = Xvector + DeltaT*Vvector;
  TMatrixD XPlusDTVT(TMatrixD::kTransposed,XPlusDTV);
  TMatrixD ChiSquared = XPlusDTVT * InvMeasErrorMatrix * XPlusDTV;
  cout << "ChiSquared = " << ChiSquared[0][0] << endl;
  
  // from combos.log
  
  double basevalue_fit[31];
  double baseerror_fit[31];
  basevalue_fit[  M_GAMMA3] =  0.1735558  ;  baseerror_fit[  M_GAMMA3] =   0.0004627    ; 
  basevalue_fit[  M_GAMMA5] =  0.1784126  ;  baseerror_fit[  M_GAMMA5] =   0.0004790    ;
  basevalue_fit[  M_GAMMA9] =  0.1090016  ;  baseerror_fit[  M_GAMMA9] =   0.0006175    ;
  basevalue_fit[ M_GAMMA10] =  0.0069099  ;  baseerror_fit[ M_GAMMA10] =   0.0002188    ;
  basevalue_fit[ M_GAMMA14] =  0.2549803  ;  baseerror_fit[ M_GAMMA14] =   0.0009240    ;
  basevalue_fit[ M_GAMMA16] =  0.0045251  ;  baseerror_fit[ M_GAMMA16] =   0.0002645    ;
  basevalue_fit[ M_GAMMA20] =  0.0924941  ;  baseerror_fit[ M_GAMMA20] =   0.0009755    ;
  basevalue_fit[ M_GAMMA23] =  0.0005807  ;  baseerror_fit[ M_GAMMA23] =   0.0002251    ;
  basevalue_fit[ M_GAMMA27] =  0.0104033  ;  baseerror_fit[ M_GAMMA27] =   0.0007089    ;
  basevalue_fit[ M_GAMMA28] =  0.0004173  ;  baseerror_fit[ M_GAMMA28] =   0.0002187    ;
  basevalue_fit[ M_GAMMA30] =  0.0010238  ;  baseerror_fit[ M_GAMMA30] =   0.0003922    ;
  basevalue_fit[ M_GAMMA35] =  0.0089630  ;  baseerror_fit[ M_GAMMA35] =   0.0003671    ;
  basevalue_fit[ M_GAMMA37] =  0.0015299  ;  baseerror_fit[ M_GAMMA37] =   0.0001606    ;
  basevalue_fit[ M_GAMMA40] =  0.0037781  ;  baseerror_fit[ M_GAMMA40] =   0.0003676    ;
  basevalue_fit[ M_GAMMA42] =  0.0015407  ;  baseerror_fit[ M_GAMMA42] =   0.0001995    ;
  basevalue_fit[ M_GAMMA47] =  0.0002412  ;  baseerror_fit[ M_GAMMA47] =   0.0000516    ;
  basevalue_fit[ M_GAMMA48] =  0.0011212  ;  baseerror_fit[ M_GAMMA48] =   0.0002454    ;
  basevalue_fit[ M_GAMMA62] =  0.0898668  ;  baseerror_fit[ M_GAMMA62] =   0.0006000    ;
  basevalue_fit[ M_GAMMA70] =  0.0269130  ;  baseerror_fit[ M_GAMMA70] =   0.0007070    ;
  basevalue_fit[ M_GAMMA77] =  0.0009063  ;  baseerror_fit[ M_GAMMA77] =   0.0003572    ;
  basevalue_fit[ M_GAMMA78] =  0.0002232  ;  baseerror_fit[ M_GAMMA78] =   0.0000499    ;
  basevalue_fit[ M_GAMMA85] =  0.0033344  ;  baseerror_fit[ M_GAMMA85] =   0.0002226    ;
  basevalue_fit[ M_GAMMA89] =  0.0007304  ;  baseerror_fit[ M_GAMMA89] =   0.0001166    ;
  basevalue_fit[ M_GAMMA93] =  0.0015310  ;  baseerror_fit[ M_GAMMA93] =   0.0000699    ;
  basevalue_fit[ M_GAMMA94] =  0.0000606  ;  baseerror_fit[ M_GAMMA94] =   0.0000183    ;
  basevalue_fit[M_GAMMA104] =  0.0001807  ;  baseerror_fit[M_GAMMA104] =   0.0000260    ;
  basevalue_fit[M_GAMMA126] =  0.0017737  ;  baseerror_fit[M_GAMMA126] =   0.0002354    ;
  basevalue_fit[M_GAMMA128] =  0.0002679  ;  baseerror_fit[M_GAMMA128] =   0.0000632    ;
  basevalue_fit[M_GAMMA150] =  0.0198604  ;  baseerror_fit[M_GAMMA150] =   0.0006376    ;
  basevalue_fit[M_GAMMA152] =  0.0040630  ;  baseerror_fit[M_GAMMA152] =   0.0004184    ;
  basevalue_fit[M_GAMMA103] = 1-0.9991899;  baseerror_fit[M_GAMMA103] =    0.0000526 ;

  double basecorr_fit[31][31];
  basecorr_fit[  M_GAMMA3][  M_GAMMA5] =      -0.1398106;
  basecorr_fit[  M_GAMMA3][  M_GAMMA9] =      -0.0442193;
  basecorr_fit[  M_GAMMA3][ M_GAMMA10] =       0.0067541;
  basecorr_fit[  M_GAMMA3][ M_GAMMA14] =      -0.1416569;
  basecorr_fit[  M_GAMMA3][ M_GAMMA16] =       0.0055145;
  basecorr_fit[  M_GAMMA3][ M_GAMMA20] =      -0.0721609;
  basecorr_fit[  M_GAMMA3][ M_GAMMA23] =       0.0002509;
  basecorr_fit[  M_GAMMA3][ M_GAMMA27] =      -0.0489258;
  basecorr_fit[  M_GAMMA3][ M_GAMMA28] =       0.0008391;
  basecorr_fit[  M_GAMMA3][ M_GAMMA30] =      -0.0299364;
  basecorr_fit[  M_GAMMA3][ M_GAMMA35] =      -0.0544814;
  basecorr_fit[  M_GAMMA3][ M_GAMMA37] =      -0.0131016;
  basecorr_fit[  M_GAMMA3][ M_GAMMA40] =      -0.0512965;
  basecorr_fit[  M_GAMMA3][ M_GAMMA42] =      -0.0148354;
  basecorr_fit[  M_GAMMA3][ M_GAMMA47] =      -0.0050993;
  basecorr_fit[  M_GAMMA3][ M_GAMMA48] =      -0.0284245;
  basecorr_fit[  M_GAMMA3][ M_GAMMA62] =      -0.0543738;
  basecorr_fit[  M_GAMMA3][ M_GAMMA70] =      -0.0261843;
  basecorr_fit[  M_GAMMA3][ M_GAMMA77] =       0.0009197;
  basecorr_fit[  M_GAMMA3][ M_GAMMA78] =       0.0018628;
  basecorr_fit[  M_GAMMA3][ M_GAMMA85] =       0.0035460;
  basecorr_fit[  M_GAMMA3][ M_GAMMA89] =       0.0075040;
  basecorr_fit[  M_GAMMA3][ M_GAMMA93] =      -0.0054906;
  basecorr_fit[  M_GAMMA3][ M_GAMMA94] =       0.0009051;
  basecorr_fit[  M_GAMMA3][M_GAMMA104] =      -0.0100057;
  basecorr_fit[  M_GAMMA3][M_GAMMA126] =      -0.0179414;
  basecorr_fit[  M_GAMMA3][M_GAMMA128] =      -0.0041118;
  basecorr_fit[  M_GAMMA3][M_GAMMA150] =      -0.0197407;
  basecorr_fit[  M_GAMMA3][M_GAMMA152] =      -0.0141597;
  basecorr_fit[  M_GAMMA5][  M_GAMMA9] =      -0.0490837;
  basecorr_fit[  M_GAMMA5][ M_GAMMA10] =       0.0102402;
  basecorr_fit[  M_GAMMA5][ M_GAMMA14] =      -0.1613112;
  basecorr_fit[  M_GAMMA5][ M_GAMMA16] =       0.0092193;
  basecorr_fit[  M_GAMMA5][ M_GAMMA20] =      -0.0793026;
  basecorr_fit[  M_GAMMA5][ M_GAMMA23] =       0.0010129;
  basecorr_fit[  M_GAMMA5][ M_GAMMA27] =      -0.0323510;
  basecorr_fit[  M_GAMMA5][ M_GAMMA28] =      -0.0008629;
  basecorr_fit[  M_GAMMA5][ M_GAMMA30] =      -0.0202811;
  basecorr_fit[  M_GAMMA5][ M_GAMMA35] =      -0.0538816;
  basecorr_fit[  M_GAMMA5][ M_GAMMA37] =      -0.0135376;
  basecorr_fit[  M_GAMMA5][ M_GAMMA40] =      -0.0510295;
  basecorr_fit[  M_GAMMA5][ M_GAMMA42] =      -0.0156125;
  basecorr_fit[  M_GAMMA5][ M_GAMMA47] =      -0.0050313;
  basecorr_fit[  M_GAMMA5][ M_GAMMA48] =      -0.0280224;
  basecorr_fit[  M_GAMMA5][ M_GAMMA62] =      -0.0678009;
  basecorr_fit[  M_GAMMA5][ M_GAMMA70] =      -0.0294123;
  basecorr_fit[  M_GAMMA5][ M_GAMMA77] =       0.0025766;
  basecorr_fit[  M_GAMMA5][ M_GAMMA78] =       0.0026196;
  basecorr_fit[  M_GAMMA5][ M_GAMMA85] =       0.0080444;
  basecorr_fit[  M_GAMMA5][ M_GAMMA89] =       0.0098168;
  basecorr_fit[  M_GAMMA5][ M_GAMMA93] =      -0.0059545;
  basecorr_fit[  M_GAMMA5][ M_GAMMA94] =       0.0012046;
  basecorr_fit[  M_GAMMA5][M_GAMMA104] =      -0.0040092;
  basecorr_fit[  M_GAMMA5][M_GAMMA126] =      -0.0180546;
  basecorr_fit[  M_GAMMA5][M_GAMMA128] =      -0.0042828;
  basecorr_fit[  M_GAMMA5][M_GAMMA150] =      -0.0202030;
  basecorr_fit[  M_GAMMA5][M_GAMMA152] =      -0.0119483;
  basecorr_fit[  M_GAMMA9][ M_GAMMA10] =      -0.2323722;
  basecorr_fit[  M_GAMMA9][ M_GAMMA14] =      -0.1640073;
  basecorr_fit[  M_GAMMA9][ M_GAMMA16] =      -0.0126115;
  basecorr_fit[  M_GAMMA9][ M_GAMMA20] =      -0.1206887;
  basecorr_fit[  M_GAMMA9][ M_GAMMA23] =       0.0030692;
  basecorr_fit[  M_GAMMA9][ M_GAMMA27] =      -0.1065742;
  basecorr_fit[  M_GAMMA9][ M_GAMMA28] =       0.0128962;
  basecorr_fit[  M_GAMMA9][ M_GAMMA30] =      -0.1161876;
  basecorr_fit[  M_GAMMA9][ M_GAMMA35] =      -0.0634975;
  basecorr_fit[  M_GAMMA9][ M_GAMMA37] =      -0.0078332;
  basecorr_fit[  M_GAMMA9][ M_GAMMA40] =      -0.0525962;
  basecorr_fit[  M_GAMMA9][ M_GAMMA42] =      -0.0037752;
  basecorr_fit[  M_GAMMA9][ M_GAMMA47] =      -0.0071043;
  basecorr_fit[  M_GAMMA9][ M_GAMMA48] =      -0.0291125;
  basecorr_fit[  M_GAMMA9][ M_GAMMA62] =       0.0257073;
  basecorr_fit[  M_GAMMA9][ M_GAMMA70] =      -0.0549364;
  basecorr_fit[  M_GAMMA9][ M_GAMMA77] =      -0.0267056;
  basecorr_fit[  M_GAMMA9][ M_GAMMA78] =       0.0020127;
  basecorr_fit[  M_GAMMA9][ M_GAMMA85] =      -0.0181087;
  basecorr_fit[  M_GAMMA9][ M_GAMMA89] =       0.0031162;
  basecorr_fit[  M_GAMMA9][ M_GAMMA93] =      -0.0014755;
  basecorr_fit[  M_GAMMA9][ M_GAMMA94] =       0.0003191;
  basecorr_fit[  M_GAMMA9][M_GAMMA104] =      -0.0071064;
  basecorr_fit[  M_GAMMA9][M_GAMMA126] =      -0.0146221;
  basecorr_fit[  M_GAMMA9][M_GAMMA128] =      -0.0019137;
  basecorr_fit[  M_GAMMA9][M_GAMMA150] =      -0.0309582;
  basecorr_fit[  M_GAMMA9][M_GAMMA152] =      -0.0489932;
  basecorr_fit[ M_GAMMA10][ M_GAMMA14] =      -0.0149955;
  basecorr_fit[ M_GAMMA10][ M_GAMMA16] =      -0.0485189;
  basecorr_fit[ M_GAMMA10][ M_GAMMA20] =      -0.0122938;
  basecorr_fit[ M_GAMMA10][ M_GAMMA23] =      -0.0376859;
  basecorr_fit[ M_GAMMA10][ M_GAMMA27] =       0.0130665;
  basecorr_fit[ M_GAMMA10][ M_GAMMA28] =      -0.0376451;
  basecorr_fit[ M_GAMMA10][ M_GAMMA30] =       0.0232747;
  basecorr_fit[ M_GAMMA10][ M_GAMMA35] =      -0.0314196;
  basecorr_fit[ M_GAMMA10][ M_GAMMA37] =      -0.0172019;
  basecorr_fit[ M_GAMMA10][ M_GAMMA40] =      -0.0277951;
  basecorr_fit[ M_GAMMA10][ M_GAMMA42] =      -0.0212636;
  basecorr_fit[ M_GAMMA10][ M_GAMMA47] =      -0.0031144;
  basecorr_fit[ M_GAMMA10][ M_GAMMA48] =      -0.0157450;
  basecorr_fit[ M_GAMMA10][ M_GAMMA62] =      -0.0369732;
  basecorr_fit[ M_GAMMA10][ M_GAMMA70] =       0.0235589;
  basecorr_fit[ M_GAMMA10][ M_GAMMA77] =       0.0099597;
  basecorr_fit[ M_GAMMA10][ M_GAMMA78] =      -0.0055575;
  basecorr_fit[ M_GAMMA10][ M_GAMMA85] =      -0.0184218;
  basecorr_fit[ M_GAMMA10][ M_GAMMA89] =      -0.0115134;
  basecorr_fit[ M_GAMMA10][ M_GAMMA93] =      -0.0087912;
  basecorr_fit[ M_GAMMA10][ M_GAMMA94] =      -0.0015598;
  basecorr_fit[ M_GAMMA10][M_GAMMA104] =       0.0015908;
  basecorr_fit[ M_GAMMA10][M_GAMMA126] =      -0.0192620;
  basecorr_fit[ M_GAMMA10][M_GAMMA128] =      -0.0070670;
  basecorr_fit[ M_GAMMA10][M_GAMMA150] =      -0.0012305;
  basecorr_fit[ M_GAMMA10][M_GAMMA152] =       0.0020641;
  basecorr_fit[ M_GAMMA14][ M_GAMMA16] =      -0.1307422;
  basecorr_fit[ M_GAMMA14][ M_GAMMA20] =      -0.4597758;
  basecorr_fit[ M_GAMMA14][ M_GAMMA23] =       0.0078579;
  basecorr_fit[ M_GAMMA14][ M_GAMMA27] =       0.0028803;
  basecorr_fit[ M_GAMMA14][ M_GAMMA28] =       0.0139797;
  basecorr_fit[ M_GAMMA14][ M_GAMMA30] =      -0.0842332;
  basecorr_fit[ M_GAMMA14][ M_GAMMA35] =      -0.0387822;
  basecorr_fit[ M_GAMMA14][ M_GAMMA37] =       0.0104592;
  basecorr_fit[ M_GAMMA14][ M_GAMMA40] =      -0.0398187;
  basecorr_fit[ M_GAMMA14][ M_GAMMA42] =       0.0156625;
  basecorr_fit[ M_GAMMA14][ M_GAMMA47] =      -0.0034857;
  basecorr_fit[ M_GAMMA14][ M_GAMMA48] =      -0.0212687;
  basecorr_fit[ M_GAMMA14][ M_GAMMA62] =      -0.0529852;
  basecorr_fit[ M_GAMMA14][ M_GAMMA70] =      -0.0883101;
  basecorr_fit[ M_GAMMA14][ M_GAMMA77] =      -0.0155489;
  basecorr_fit[ M_GAMMA14][ M_GAMMA78] =       0.0054345;
  basecorr_fit[ M_GAMMA14][ M_GAMMA85] =      -0.0013909;
  basecorr_fit[ M_GAMMA14][ M_GAMMA89] =       0.0116120;
  basecorr_fit[ M_GAMMA14][ M_GAMMA93] =      -0.0064760;
  basecorr_fit[ M_GAMMA14][ M_GAMMA94] =       0.0015073;
  basecorr_fit[ M_GAMMA14][M_GAMMA104] =      -0.0115092;
  basecorr_fit[ M_GAMMA14][M_GAMMA126] =      -0.0139159;
  basecorr_fit[ M_GAMMA14][M_GAMMA128] =       0.0015531;
  basecorr_fit[ M_GAMMA14][M_GAMMA150] =      -0.0357123;
  basecorr_fit[ M_GAMMA14][M_GAMMA152] =      -0.0306701;
  basecorr_fit[ M_GAMMA16][ M_GAMMA20] =       0.0010033;
  basecorr_fit[ M_GAMMA16][ M_GAMMA23] =      -0.2005798;
  basecorr_fit[ M_GAMMA16][ M_GAMMA27] =       0.0155482;
  basecorr_fit[ M_GAMMA16][ M_GAMMA28] =      -0.1893705;
  basecorr_fit[ M_GAMMA16][ M_GAMMA30] =       0.0013112;
  basecorr_fit[ M_GAMMA16][ M_GAMMA35] =      -0.0074251;
  basecorr_fit[ M_GAMMA16][ M_GAMMA37] =      -0.1100590;
  basecorr_fit[ M_GAMMA16][ M_GAMMA40] =       0.0077956;
  basecorr_fit[ M_GAMMA16][ M_GAMMA42] =      -0.1469530;
  basecorr_fit[ M_GAMMA16][ M_GAMMA47] =      -0.0008203;
  basecorr_fit[ M_GAMMA16][ M_GAMMA48] =      -0.0050235;
  basecorr_fit[ M_GAMMA16][ M_GAMMA62] =      -0.0208419;
  basecorr_fit[ M_GAMMA16][ M_GAMMA70] =       0.0177981;
  basecorr_fit[ M_GAMMA16][ M_GAMMA77] =       0.0053657;
  basecorr_fit[ M_GAMMA16][ M_GAMMA78] =      -0.0045973;
  basecorr_fit[ M_GAMMA16][ M_GAMMA85] =      -0.0169157;
  basecorr_fit[ M_GAMMA16][ M_GAMMA89] =      -0.0063336;
  basecorr_fit[ M_GAMMA16][ M_GAMMA93] =      -0.0068489;
  basecorr_fit[ M_GAMMA16][ M_GAMMA94] =      -0.0014097;
  basecorr_fit[ M_GAMMA16][M_GAMMA104] =       0.0017907;
  basecorr_fit[ M_GAMMA16][M_GAMMA126] =      -0.0137086;
  basecorr_fit[ M_GAMMA16][M_GAMMA128] =      -0.0342514;
  basecorr_fit[ M_GAMMA16][M_GAMMA150] =      -0.0012827;
  basecorr_fit[ M_GAMMA16][M_GAMMA152] =      -0.0010592;
  basecorr_fit[ M_GAMMA20][ M_GAMMA23] =      -0.0901599;
  basecorr_fit[ M_GAMMA20][ M_GAMMA27] =      -0.3775189;
  basecorr_fit[ M_GAMMA20][ M_GAMMA28] =      -0.0310330;
  basecorr_fit[ M_GAMMA20][ M_GAMMA30] =       0.0734087;
  basecorr_fit[ M_GAMMA20][ M_GAMMA35] =      -0.0622588;
  basecorr_fit[ M_GAMMA20][ M_GAMMA37] =       0.0031267;
  basecorr_fit[ M_GAMMA20][ M_GAMMA40] =      -0.0550191;
  basecorr_fit[ M_GAMMA20][ M_GAMMA42] =       0.0099081;
  basecorr_fit[ M_GAMMA20][ M_GAMMA47] =      -0.0056730;
  basecorr_fit[ M_GAMMA20][ M_GAMMA48] =      -0.0310932;
  basecorr_fit[ M_GAMMA20][ M_GAMMA62] =      -0.1155246;
  basecorr_fit[ M_GAMMA20][ M_GAMMA70] =       0.0172500;
  basecorr_fit[ M_GAMMA20][ M_GAMMA77] =       0.0052290;
  basecorr_fit[ M_GAMMA20][ M_GAMMA78] =      -0.0060619;
  basecorr_fit[ M_GAMMA20][ M_GAMMA85] =      -0.0187852;
  basecorr_fit[ M_GAMMA20][ M_GAMMA89] =      -0.0122292;
  basecorr_fit[ M_GAMMA20][ M_GAMMA93] =      -0.0179002;
  basecorr_fit[ M_GAMMA20][ M_GAMMA94] =      -0.0015383;
  basecorr_fit[ M_GAMMA20][M_GAMMA104] =       0.0070302;
  basecorr_fit[ M_GAMMA20][M_GAMMA126] =      -0.0522924;
  basecorr_fit[ M_GAMMA20][M_GAMMA128] =      -0.0039721;
  basecorr_fit[ M_GAMMA20][M_GAMMA150] =      -0.0085461;
  basecorr_fit[ M_GAMMA20][M_GAMMA152] =      -0.0157234;
  basecorr_fit[ M_GAMMA23][ M_GAMMA27] =       0.0025790;
  basecorr_fit[ M_GAMMA23][ M_GAMMA28] =      -0.1549596;
  basecorr_fit[ M_GAMMA23][ M_GAMMA30] =      -0.0156593;
  basecorr_fit[ M_GAMMA23][ M_GAMMA35] =      -0.0054735;
  basecorr_fit[ M_GAMMA23][ M_GAMMA37] =      -0.0853863;
  basecorr_fit[ M_GAMMA23][ M_GAMMA40] =       0.0064869;
  basecorr_fit[ M_GAMMA23][ M_GAMMA42] =      -0.1139547;
  basecorr_fit[ M_GAMMA23][ M_GAMMA47] =      -0.0006031;
  basecorr_fit[ M_GAMMA23][ M_GAMMA48] =      -0.0033207;
  basecorr_fit[ M_GAMMA23][ M_GAMMA62] =      -0.0037962;
  basecorr_fit[ M_GAMMA23][ M_GAMMA70] =       0.0047809;
  basecorr_fit[ M_GAMMA23][ M_GAMMA77] =       0.0010959;
  basecorr_fit[ M_GAMMA23][ M_GAMMA78] =      -0.0026823;
  basecorr_fit[ M_GAMMA23][ M_GAMMA85] =      -0.0113549;
  basecorr_fit[ M_GAMMA23][ M_GAMMA89] =      -0.0026994;
  basecorr_fit[ M_GAMMA23][ M_GAMMA93] =      -0.0034590;
  basecorr_fit[ M_GAMMA23][ M_GAMMA94] =      -0.0008323;
  basecorr_fit[ M_GAMMA23][M_GAMMA104] =       0.0000890;
  basecorr_fit[ M_GAMMA23][M_GAMMA126] =      -0.0117667;
  basecorr_fit[ M_GAMMA23][M_GAMMA128] =      -0.0271396;
  basecorr_fit[ M_GAMMA23][M_GAMMA150] =      -0.0035027;
  basecorr_fit[ M_GAMMA23][M_GAMMA152] =      -0.0047502;
  basecorr_fit[ M_GAMMA27][ M_GAMMA28] =      -0.1068912;
  basecorr_fit[ M_GAMMA27][ M_GAMMA30] =      -0.4317599;
  basecorr_fit[ M_GAMMA27][ M_GAMMA35] =      -0.0158934;
  basecorr_fit[ M_GAMMA27][ M_GAMMA37] =       0.0095753;
  basecorr_fit[ M_GAMMA27][ M_GAMMA40] =      -0.0375757;
  basecorr_fit[ M_GAMMA27][ M_GAMMA42] =       0.0045858;
  basecorr_fit[ M_GAMMA27][ M_GAMMA47] =      -0.0010823;
  basecorr_fit[ M_GAMMA27][ M_GAMMA48] =      -0.0065164;
  basecorr_fit[ M_GAMMA27][ M_GAMMA62] =      -0.0774282;
  basecorr_fit[ M_GAMMA27][ M_GAMMA70] =      -0.0034792;
  basecorr_fit[ M_GAMMA27][ M_GAMMA77] =       0.0180688;
  basecorr_fit[ M_GAMMA27][ M_GAMMA78] =      -0.0023882;
  basecorr_fit[ M_GAMMA27][ M_GAMMA85] =       0.0031287;
  basecorr_fit[ M_GAMMA27][ M_GAMMA89] =      -0.0007924;
  basecorr_fit[ M_GAMMA27][ M_GAMMA93] =      -0.0081831;
  basecorr_fit[ M_GAMMA27][ M_GAMMA94] =      -0.0001695;
  basecorr_fit[ M_GAMMA27][M_GAMMA104] =       0.0021012;
  basecorr_fit[ M_GAMMA27][M_GAMMA126] =       0.0078432;
  basecorr_fit[ M_GAMMA27][M_GAMMA128] =      -0.0076877;
  basecorr_fit[ M_GAMMA27][M_GAMMA150] =      -0.0040996;
  basecorr_fit[ M_GAMMA27][M_GAMMA152] =       0.0211833;
  basecorr_fit[ M_GAMMA28][ M_GAMMA30] =       0.0197242;
  basecorr_fit[ M_GAMMA28][ M_GAMMA35] =      -0.0053752;
  basecorr_fit[ M_GAMMA28][ M_GAMMA37] =      -0.0821974;
  basecorr_fit[ M_GAMMA28][ M_GAMMA40] =       0.0011575;
  basecorr_fit[ M_GAMMA28][ M_GAMMA42] =      -0.1118565;
  basecorr_fit[ M_GAMMA28][ M_GAMMA47] =      -0.0005425;
  basecorr_fit[ M_GAMMA28][ M_GAMMA48] =      -0.0029206;
  basecorr_fit[ M_GAMMA28][ M_GAMMA62] =       0.0035802;
  basecorr_fit[ M_GAMMA28][ M_GAMMA70] =       0.0036478;
  basecorr_fit[ M_GAMMA28][ M_GAMMA77] =      -0.0016260;
  basecorr_fit[ M_GAMMA28][ M_GAMMA78] =      -0.0024801;
  basecorr_fit[ M_GAMMA28][ M_GAMMA85] =      -0.0110067;
  basecorr_fit[ M_GAMMA28][ M_GAMMA89] =      -0.0019347;
  basecorr_fit[ M_GAMMA28][ M_GAMMA93] =      -0.0025159;
  basecorr_fit[ M_GAMMA28][ M_GAMMA94] =      -0.0007562;
  basecorr_fit[ M_GAMMA28][M_GAMMA104] =      -0.0000654;
  basecorr_fit[ M_GAMMA28][M_GAMMA126] =      -0.0149136;
  basecorr_fit[ M_GAMMA28][M_GAMMA128] =      -0.0282800;
  basecorr_fit[ M_GAMMA28][M_GAMMA150] =      -0.0038002;
  basecorr_fit[ M_GAMMA28][M_GAMMA152] =      -0.0087779;
  basecorr_fit[ M_GAMMA30][ M_GAMMA35] =       0.0026979;
  basecorr_fit[ M_GAMMA30][ M_GAMMA37] =      -0.0014203;
  basecorr_fit[ M_GAMMA30][ M_GAMMA40] =       0.0107282;
  basecorr_fit[ M_GAMMA30][ M_GAMMA42] =       0.0013961;
  basecorr_fit[ M_GAMMA30][ M_GAMMA47] =       0.0002966;
  basecorr_fit[ M_GAMMA30][ M_GAMMA48] =       0.0007288;
  basecorr_fit[ M_GAMMA30][ M_GAMMA62] =      -0.0510181;
  basecorr_fit[ M_GAMMA30][ M_GAMMA70] =       0.0344566;
  basecorr_fit[ M_GAMMA30][ M_GAMMA77] =       0.0285155;
  basecorr_fit[ M_GAMMA30][ M_GAMMA78] =      -0.0041128;
  basecorr_fit[ M_GAMMA30][ M_GAMMA85] =      -0.0010730;
  basecorr_fit[ M_GAMMA30][ M_GAMMA89] =      -0.0080199;
  basecorr_fit[ M_GAMMA30][ M_GAMMA93] =      -0.0061413;
  basecorr_fit[ M_GAMMA30][ M_GAMMA94] =      -0.0009521;
  basecorr_fit[ M_GAMMA30][M_GAMMA104] =      -0.0081148;
  basecorr_fit[ M_GAMMA30][M_GAMMA126] =      -0.0636928;
  basecorr_fit[ M_GAMMA30][M_GAMMA128] =       0.0014245;
  basecorr_fit[ M_GAMMA30][M_GAMMA150] =       0.0113167;
  basecorr_fit[ M_GAMMA30][M_GAMMA152] =       0.0248590;
  basecorr_fit[ M_GAMMA35][ M_GAMMA37] =      -0.1296639;
  basecorr_fit[ M_GAMMA35][ M_GAMMA40] =      -0.1536118;
  basecorr_fit[ M_GAMMA35][ M_GAMMA42] =      -0.0120234;
  basecorr_fit[ M_GAMMA35][ M_GAMMA47] =      -0.0263909;
  basecorr_fit[ M_GAMMA35][ M_GAMMA48] =      -0.1428663;
  basecorr_fit[ M_GAMMA35][ M_GAMMA62] =      -0.0478264;
  basecorr_fit[ M_GAMMA35][ M_GAMMA70] =       0.0278911;
  basecorr_fit[ M_GAMMA35][ M_GAMMA77] =       0.0099176;
  basecorr_fit[ M_GAMMA35][ M_GAMMA78] =      -0.0092941;
  basecorr_fit[ M_GAMMA35][ M_GAMMA85] =      -0.0379966;
  basecorr_fit[ M_GAMMA35][ M_GAMMA89] =      -0.0198099;
  basecorr_fit[ M_GAMMA35][ M_GAMMA93] =      -0.0149233;
  basecorr_fit[ M_GAMMA35][ M_GAMMA94] =      -0.0025914;
  basecorr_fit[ M_GAMMA35][M_GAMMA104] =      -0.0003497;
  basecorr_fit[ M_GAMMA35][M_GAMMA126] =      -0.0424515;
  basecorr_fit[ M_GAMMA35][M_GAMMA128] =      -0.0038406;
  basecorr_fit[ M_GAMMA35][M_GAMMA150] =      -0.0132005;
  basecorr_fit[ M_GAMMA35][M_GAMMA152] =      -0.0139249;
  basecorr_fit[ M_GAMMA37][ M_GAMMA40] =       0.0015936;
  basecorr_fit[ M_GAMMA37][ M_GAMMA42] =      -0.1593902;
  basecorr_fit[ M_GAMMA37][ M_GAMMA47] =      -0.0054051;
  basecorr_fit[ M_GAMMA37][ M_GAMMA48] =      -0.0289324;
  basecorr_fit[ M_GAMMA37][ M_GAMMA62] =      -0.0054287;
  basecorr_fit[ M_GAMMA37][ M_GAMMA70] =       0.0014862;
  basecorr_fit[ M_GAMMA37][ M_GAMMA77] =       0.0010987;
  basecorr_fit[ M_GAMMA37][ M_GAMMA78] =      -0.0003540;
  basecorr_fit[ M_GAMMA37][ M_GAMMA85] =      -0.0043532;
  basecorr_fit[ M_GAMMA37][ M_GAMMA89] =      -0.0000820;
  basecorr_fit[ M_GAMMA37][ M_GAMMA93] =      -0.0007662;
  basecorr_fit[ M_GAMMA37][ M_GAMMA94] =      -0.0000165;
  basecorr_fit[ M_GAMMA37][M_GAMMA104] =      -0.0004131;
  basecorr_fit[ M_GAMMA37][M_GAMMA126] =      -0.0025994;
  basecorr_fit[ M_GAMMA37][M_GAMMA128] =      -0.0155083;
  basecorr_fit[ M_GAMMA37][M_GAMMA150] =      -0.0015556;
  basecorr_fit[ M_GAMMA37][M_GAMMA152] =      -0.0009196;
  basecorr_fit[ M_GAMMA40][ M_GAMMA42] =      -0.1984652;
  basecorr_fit[ M_GAMMA40][ M_GAMMA47] =      -0.0246471;
  basecorr_fit[ M_GAMMA40][ M_GAMMA48] =      -0.1367763;
  basecorr_fit[ M_GAMMA40][ M_GAMMA62] =      -0.0446843;
  basecorr_fit[ M_GAMMA40][ M_GAMMA70] =       0.0268226;
  basecorr_fit[ M_GAMMA40][ M_GAMMA77] =       0.0090517;
  basecorr_fit[ M_GAMMA40][ M_GAMMA78] =      -0.0090162;
  basecorr_fit[ M_GAMMA40][ M_GAMMA85] =      -0.0369556;
  basecorr_fit[ M_GAMMA40][ M_GAMMA89] =      -0.0193487;
  basecorr_fit[ M_GAMMA40][ M_GAMMA93] =      -0.0144224;
  basecorr_fit[ M_GAMMA40][ M_GAMMA94] =      -0.0025363;
  basecorr_fit[ M_GAMMA40][M_GAMMA104] =      -0.0003939;
  basecorr_fit[ M_GAMMA40][M_GAMMA126] =      -0.0416212;
  basecorr_fit[ M_GAMMA40][M_GAMMA128] =      -0.0020639;
  basecorr_fit[ M_GAMMA40][M_GAMMA150] =      -0.0129337;
  basecorr_fit[ M_GAMMA40][M_GAMMA152] =      -0.0143383;
  basecorr_fit[ M_GAMMA42][ M_GAMMA47] =      -0.0057833;
  basecorr_fit[ M_GAMMA42][ M_GAMMA48] =      -0.0323030;
  basecorr_fit[ M_GAMMA42][ M_GAMMA62] =      -0.0043846;
  basecorr_fit[ M_GAMMA42][ M_GAMMA70] =       0.0005407;
  basecorr_fit[ M_GAMMA42][ M_GAMMA77] =       0.0007784;
  basecorr_fit[ M_GAMMA42][ M_GAMMA78] =      -0.0000166;
  basecorr_fit[ M_GAMMA42][ M_GAMMA85] =      -0.0041714;
  basecorr_fit[ M_GAMMA42][ M_GAMMA89] =       0.0008477;
  basecorr_fit[ M_GAMMA42][ M_GAMMA93] =      -0.0002794;
  basecorr_fit[ M_GAMMA42][ M_GAMMA94] =       0.0001006;
  basecorr_fit[ M_GAMMA42][M_GAMMA104] =      -0.0005832;
  basecorr_fit[ M_GAMMA42][M_GAMMA126] =      -0.0016398;
  basecorr_fit[ M_GAMMA42][M_GAMMA128] =      -0.0207592;
  basecorr_fit[ M_GAMMA42][M_GAMMA150] =      -0.0015508;
  basecorr_fit[ M_GAMMA42][M_GAMMA152] =      -0.0009290;
  basecorr_fit[ M_GAMMA47][ M_GAMMA48] =      -0.0287511;
  basecorr_fit[ M_GAMMA47][ M_GAMMA62] =      -0.0046500;
  basecorr_fit[ M_GAMMA47][ M_GAMMA70] =       0.0026305;
  basecorr_fit[ M_GAMMA47][ M_GAMMA77] =       0.0009991;
  basecorr_fit[ M_GAMMA47][ M_GAMMA78] =      -0.0008822;
  basecorr_fit[ M_GAMMA47][ M_GAMMA85] =      -0.0035703;
  basecorr_fit[ M_GAMMA47][ M_GAMMA89] =      -0.0018730;
  basecorr_fit[ M_GAMMA47][ M_GAMMA93] =      -0.0014191;
  basecorr_fit[ M_GAMMA47][ M_GAMMA94] =      -0.0002448;
  basecorr_fit[ M_GAMMA47][M_GAMMA104] =      -0.0000231;
  basecorr_fit[ M_GAMMA47][M_GAMMA126] =      -0.0039660;
  basecorr_fit[ M_GAMMA47][M_GAMMA128] =      -0.0003720;
  basecorr_fit[ M_GAMMA47][M_GAMMA150] =      -0.0012245;
  basecorr_fit[ M_GAMMA47][M_GAMMA152] =      -0.0012061;
  basecorr_fit[ M_GAMMA48][ M_GAMMA62] =      -0.0277660;
  basecorr_fit[ M_GAMMA48][ M_GAMMA70] =       0.0128549;
  basecorr_fit[ M_GAMMA48][ M_GAMMA77] =       0.0059774;
  basecorr_fit[ M_GAMMA48][ M_GAMMA78] =      -0.0050078;
  basecorr_fit[ M_GAMMA48][ M_GAMMA85] =      -0.0204251;
  basecorr_fit[ M_GAMMA48][ M_GAMMA89] =      -0.0105436;
  basecorr_fit[ M_GAMMA48][ M_GAMMA93] =      -0.0082504;
  basecorr_fit[ M_GAMMA48][ M_GAMMA94] =      -0.0013775;
  basecorr_fit[ M_GAMMA48][M_GAMMA104] =      -0.0001653;
  basecorr_fit[ M_GAMMA48][M_GAMMA126] =      -0.0220764;
  basecorr_fit[ M_GAMMA48][M_GAMMA128] =      -0.0021177;
  basecorr_fit[ M_GAMMA48][M_GAMMA150] =      -0.0074874;
  basecorr_fit[ M_GAMMA48][M_GAMMA152] =      -0.0062548;
  basecorr_fit[ M_GAMMA62][ M_GAMMA70] =      -0.2106392;
  basecorr_fit[ M_GAMMA62][ M_GAMMA77] =      -0.0064639;
  basecorr_fit[ M_GAMMA62][ M_GAMMA78] =      -0.0139997;
  basecorr_fit[ M_GAMMA62][ M_GAMMA85] =      -0.1706340;
  basecorr_fit[ M_GAMMA62][ M_GAMMA89] =      -0.0451701;
  basecorr_fit[ M_GAMMA62][ M_GAMMA93] =       0.0740378;
  basecorr_fit[ M_GAMMA62][ M_GAMMA94] =      -0.0065829;
  basecorr_fit[ M_GAMMA62][M_GAMMA104] =      -0.0080569;
  basecorr_fit[ M_GAMMA62][M_GAMMA126] =      -0.0199451;
  basecorr_fit[ M_GAMMA62][M_GAMMA128] =      -0.0032546;
  basecorr_fit[ M_GAMMA62][M_GAMMA150] =      -0.0946256;
  basecorr_fit[ M_GAMMA62][M_GAMMA152] =      -0.0227512;
  basecorr_fit[ M_GAMMA70][ M_GAMMA77] =      -0.0751317;
  basecorr_fit[ M_GAMMA70][ M_GAMMA78] =      -0.0172115;
  basecorr_fit[ M_GAMMA70][ M_GAMMA85] =      -0.0238256;
  basecorr_fit[ M_GAMMA70][ M_GAMMA89] =      -0.0834838;
  basecorr_fit[ M_GAMMA70][ M_GAMMA93] =      -0.0315776;
  basecorr_fit[ M_GAMMA70][ M_GAMMA94] =      -0.0103175;
  basecorr_fit[ M_GAMMA70][M_GAMMA104] =       0.0011510;
  basecorr_fit[ M_GAMMA70][M_GAMMA126] =       0.0098975;
  basecorr_fit[ M_GAMMA70][M_GAMMA128] =       0.0011827;
  basecorr_fit[ M_GAMMA70][M_GAMMA150] =      -0.6502507;
  basecorr_fit[ M_GAMMA70][M_GAMMA152] =      -0.1066436;
  basecorr_fit[ M_GAMMA77][ M_GAMMA78] =      -0.0068512;
  basecorr_fit[ M_GAMMA77][ M_GAMMA85] =       0.0025107;
  basecorr_fit[ M_GAMMA77][ M_GAMMA89] =      -0.0051719;
  basecorr_fit[ M_GAMMA77][ M_GAMMA93] =      -0.0001972;
  basecorr_fit[ M_GAMMA77][ M_GAMMA94] =      -0.0006944;
  basecorr_fit[ M_GAMMA77][M_GAMMA104] =       0.0038914;
  basecorr_fit[ M_GAMMA77][M_GAMMA126] =      -0.1501020;
  basecorr_fit[ M_GAMMA77][M_GAMMA128] =       0.0002415;
  basecorr_fit[ M_GAMMA77][M_GAMMA150] =      -0.0241880;
  basecorr_fit[ M_GAMMA77][M_GAMMA152] =      -0.6249931;
  basecorr_fit[ M_GAMMA78][ M_GAMMA85] =      -0.0081483;
  basecorr_fit[ M_GAMMA78][ M_GAMMA89] =      -0.0077215;
  basecorr_fit[ M_GAMMA78][ M_GAMMA93] =      -0.0036149;
  basecorr_fit[ M_GAMMA78][ M_GAMMA94] =      -0.0009975;
  basecorr_fit[ M_GAMMA78][M_GAMMA104] =       0.0008682;
  basecorr_fit[ M_GAMMA78][M_GAMMA126] =      -0.0048820;
  basecorr_fit[ M_GAMMA78][M_GAMMA128] =      -0.0006644;
  basecorr_fit[ M_GAMMA78][M_GAMMA150] =      -0.0083806;
  basecorr_fit[ M_GAMMA78][M_GAMMA152] =      -0.0116850;
  basecorr_fit[ M_GAMMA85][ M_GAMMA89] =      -0.0297079;
  basecorr_fit[ M_GAMMA85][ M_GAMMA93] =      -0.0970376;
  basecorr_fit[ M_GAMMA85][ M_GAMMA94] =      -0.0021084;
  basecorr_fit[ M_GAMMA85][M_GAMMA104] =       0.0006996;
  basecorr_fit[ M_GAMMA85][M_GAMMA126] =      -0.0212952;
  basecorr_fit[ M_GAMMA85][M_GAMMA128] =      -0.0029553;
  basecorr_fit[ M_GAMMA85][M_GAMMA150] =      -0.0202222;
  basecorr_fit[ M_GAMMA85][M_GAMMA152] =      -0.0091306;
  basecorr_fit[ M_GAMMA89][ M_GAMMA93] =      -0.0097805;
  basecorr_fit[ M_GAMMA89][ M_GAMMA94] =      -0.0314687;
  basecorr_fit[ M_GAMMA89][M_GAMMA104] =       0.0012733;
  basecorr_fit[ M_GAMMA89][M_GAMMA126] =      -0.0105441;
  basecorr_fit[ M_GAMMA89][M_GAMMA128] =      -0.1240832;
  basecorr_fit[ M_GAMMA89][M_GAMMA150] =       0.0194173;
  basecorr_fit[ M_GAMMA89][M_GAMMA152] =      -0.0128608;
  basecorr_fit[ M_GAMMA93][ M_GAMMA94] =      -0.0071562;
  basecorr_fit[ M_GAMMA93][M_GAMMA104] =      -0.0007602;
  basecorr_fit[ M_GAMMA93][M_GAMMA126] =      -0.0074842;
  basecorr_fit[ M_GAMMA93][M_GAMMA128] =      -0.0010640;
  basecorr_fit[ M_GAMMA93][M_GAMMA150] =      -0.0136961;
  basecorr_fit[ M_GAMMA93][M_GAMMA152] =      -0.0049298;
  basecorr_fit[ M_GAMMA94][M_GAMMA104] =       0.0001665;
  basecorr_fit[ M_GAMMA94][M_GAMMA126] =      -0.0013833;
  basecorr_fit[ M_GAMMA94][M_GAMMA128] =      -0.0001796;
  basecorr_fit[ M_GAMMA94][M_GAMMA150] =       0.0025805;
  basecorr_fit[ M_GAMMA94][M_GAMMA152] =      -0.0017060;
  basecorr_fit[M_GAMMA104][M_GAMMA126] =       0.0004322;
  basecorr_fit[M_GAMMA104][M_GAMMA128] =      -0.0001225;
  basecorr_fit[M_GAMMA104][M_GAMMA150] =       0.0003904;
  basecorr_fit[M_GAMMA104][M_GAMMA152] =       0.0049262;
  basecorr_fit[M_GAMMA126][M_GAMMA128] =      -0.0032730;
  basecorr_fit[M_GAMMA126][M_GAMMA150] =      -0.0078631;
  basecorr_fit[M_GAMMA126][M_GAMMA152] =      -0.0105429;
  basecorr_fit[M_GAMMA128][M_GAMMA150] =      -0.0012260;
  basecorr_fit[M_GAMMA128][M_GAMMA152] =      -0.0016522;
  basecorr_fit[M_GAMMA150][M_GAMMA152] =      -0.0353700;

  for (i=0;i<31;++i) for (j=0;j<31;++j) if (i==j) basecorr_fit[i][j]=1;
  for (i=0;i<30;++i) for (j=0;j<30;++j) if (i>j)  basecorr_fit[i][j]=basecorr_fit[j][i];
  for (i=0;i<31;++i) {
    cout << "basecorr["<<i<<"]=";
    for (j=0;j<31;++j) {
      cout << Form("%8.4g ",basecorr_fit[i][j]);
      if (j%10==9) cout << endl;
    }
    cout << endl; 
  }
  double basecov_fit[31][31];
  for (i=0;i<30;++i){
    for (j=0;j<30;++j){
      basecov_fit[i][j] = basecorr_fit[i][j]*baseerror_fit[i]*baseerror_fit[j];
    }
  }
  for (i=0;i<30;++i){
    basecov_fit[i][30] = 0;
    for (int j=0;j<30;++j) {
      basecov_fit[i][30] -= basecov_fit[i][j];
    }
  }
  for (i=0;i<30;++i){
    basecov_fit[30][i] = basecov_fit[i][30];
  }
  basecov_fit[30][30] = baseerror_fit[30]*baseerror_fit[30];
  for (i=0;i<31;++i) {
    cout << "basecov["<<i<<"]=";
    for (j=0;j<31;++j) {
      cout << Form("%8.4g ",basecov_fit[i][j]);
      if (j%10==9) cout << endl;
    }
    cout << endl; 
  }
  for (i=0;i<30;++i){
    basecorr_fit[i][30] = basecov_fit[i][30] / (baseerror_fit[i]*baseerror_fit[30]);
    basecorr_fit[30][i] = basecov_fit[i][30] / (baseerror_fit[i]*baseerror_fit[30]);
  }
  //
  vector<double> FitValue_part_re[nmeas];
  double FitValue_num_re[nmeas];
  double FitValue_den_re[nmeas];
  double FitValue_re[nmeas];
  for (i=0;i<nmeas;++i) {
    vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
    inode=it-nodename.begin();
    FitValue_num_re[i]=0; // sum_j=1,n alpha_j P_j
    if (node_num_npar[inode]>0) {
      for (ipar=0;ipar<node_num_npar[inode];++ipar){
	int parm=node_num_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	FitValue_num_re[i]+=(node_num_coef[inode].at(ipar))*(basevalue_fit[quan-1]);
      }
    }
    FitValue_den_re[i]=0; // sum_k=1,m beta_k P_k
    if (node_den_npar[inode]>0) {
      for (ipar=0;ipar<node_den_npar[inode];++ipar){
	int parm=node_den_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	FitValue_den_re[i]+=(node_den_coef[inode].at(ipar))*(basevalue_fit[quan-1]);
      }
    }
    FitValue_re[i] = (FitValue_den_re[i]!=0) ? FitValue_num_re[i]/FitValue_den_re[i] : FitValue_num_re[i];
    //
    for (ipar=0;ipar<node_parm[inode].size();++ipar){
      int parm=node_parm[inode].at(ipar);
      vector<int>::iterator it_num=find(node_num_parm[inode].begin(),node_num_parm[inode].end(),parm); bool is_in_num = it_num != node_num_parm[inode].end();
      vector<int>::iterator it_den=find(node_den_parm[inode].begin(),node_den_parm[inode].end(),parm); bool is_in_den = it_den != node_den_parm[inode].end();
      double partial=0;
      if (node_den_npar[inode]==0 && is_in_num) {//case 1
	partial = node_num_coef[inode].at(it_num - node_num_parm[inode].begin());
      } else if ( is_in_num && !is_in_den) { // case 2a
	partial = (1./FitValue_den_re[i]) * (node_num_coef[inode].at(it_num - node_num_parm[inode].begin()));
      } else if (!is_in_num &&  is_in_den) { // case 2b
	partial = -1. * (FitValue_num_re[i]/(FitValue_den_re[i]*FitValue_den_re[i])) * (node_den_coef[inode].at(it_den - node_den_parm[inode].begin()));
      } else if ( is_in_num &&  is_in_den) { // case 2c
	partial = (1./FitValue_den_re[i]) * (node_num_coef[inode].at(it_num - node_num_parm[inode].begin())) -1. * (FitValue_num_re[i]/(FitValue_den_re[i]*FitValue_den_re[i])) * (node_den_coef[inode].at(it_den - node_den_parm[inode].begin()));
      }
      FitValue_part_re[i].push_back(partial); // <--
    }
  }
  //
  double FitError_re[nmeas];
  TMatrixD FitErrorMatrix_re(nmeas,nmeas);
  for (i=0;i<nmeas;++i) {
    vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
    inode=it-nodename.begin();
    for (j=0;j<nmeas;++j) {
      vector<string>::iterator jt=find(nodename.begin(),nodename.end(),measnodename[j]);      
      jnode=jt-nodename.begin();
      FitErrorMatrix_re[i][j] = 0;
      for (ipar = 0; ipar < node_parm[inode].size(); ++ipar) {
	int iquan=node_quan[inode].at(ipar);
        //double ipartial=node_part[inode].at(ipar);
	double ipartial=FitValue_part_re[i].at(ipar);
	for (jpar = 0; jpar < node_parm[jnode].size(); ++jpar) {
	  int jquan=node_quan[jnode].at(jpar);
          //double jpartial=node_part[jnode].at(jpar);
	  double jpartial=FitValue_part_re[j].at(jpar);
//	  cout << "i = " << i << " j = " << j << " ipartial = " << ipartial << " jpartial = " << jpartial << " iquan = " << iquan << " jquan = " << jquan << " "
//	       << "basefiterror_i = " << baseerror_fit[iquan-1] << " " 
//	       << "basefiterror_j = " << baseerror_fit[jquan-1] << " "
//	       << "basefitcorr_ij = " << basecorr_fit[iquan-1][jquan-1]
//	       << endl;
	  FitErrorMatrix_re[i][j] += ipartial * jpartial *  baseerror_fit[iquan-1] * baseerror_fit[jquan-1] * basecorr_fit[iquan-1][jquan-1];
	}
      }
    }
    //
    FitError_re[i] = TMath::Sqrt(FitErrorMatrix_re[i][i]);
  }

  // from combos_no-weak.log

  double basevalue_fit_noweak[31];
  double baseerror_fit_noweak[31];
  basevalue_fit_noweak[  M_GAMMA3] =  0.1734990 ;  baseerror_fit_noweak[  M_GAMMA3] =    0.0004644 ; 
  basevalue_fit_noweak[  M_GAMMA5] =  0.1783781 ;  baseerror_fit_noweak[  M_GAMMA5] =    0.0004809 ;
  basevalue_fit_noweak[  M_GAMMA9] =  0.1084115 ;  baseerror_fit_noweak[  M_GAMMA9] =    0.0006542 ;
  basevalue_fit_noweak[ M_GAMMA10] =  0.0068695 ;  baseerror_fit_noweak[ M_GAMMA10] =    0.0002265 ;
  basevalue_fit_noweak[ M_GAMMA14] =  0.2546999 ;  baseerror_fit_noweak[ M_GAMMA14] =    0.0009670 ;
  basevalue_fit_noweak[ M_GAMMA16] =  0.0045246 ;  baseerror_fit_noweak[ M_GAMMA16] =    0.0002658 ;
  basevalue_fit_noweak[ M_GAMMA20] =  0.0934882 ;  baseerror_fit_noweak[ M_GAMMA20] =    0.0010503 ;
  basevalue_fit_noweak[ M_GAMMA23] =  0.0005920 ;  baseerror_fit_noweak[ M_GAMMA23] =    0.0002313 ;
  basevalue_fit_noweak[ M_GAMMA27] =  0.0102929 ;  baseerror_fit_noweak[ M_GAMMA27] =    0.0007231 ;
  basevalue_fit_noweak[ M_GAMMA28] =  0.0004106 ;  baseerror_fit_noweak[ M_GAMMA28] =    0.0002194 ;
  basevalue_fit_noweak[ M_GAMMA30] =  0.0011533 ;  baseerror_fit_noweak[ M_GAMMA30] =    0.0003942 ;
  basevalue_fit_noweak[ M_GAMMA35] =  0.0089820 ;  baseerror_fit_noweak[ M_GAMMA35] =    0.0003677 ;
  basevalue_fit_noweak[ M_GAMMA37] =  0.0015267 ;  baseerror_fit_noweak[ M_GAMMA37] =    0.0001608 ;
  basevalue_fit_noweak[ M_GAMMA40] =  0.0038037 ;  baseerror_fit_noweak[ M_GAMMA40] =    0.0003681 ;
  basevalue_fit_noweak[ M_GAMMA42] =  0.0015363 ;  baseerror_fit_noweak[ M_GAMMA42] =    0.0002000 ;
  basevalue_fit_noweak[ M_GAMMA47] =  0.0002412 ;  baseerror_fit_noweak[ M_GAMMA47] =    0.0000516 ;
  basevalue_fit_noweak[ M_GAMMA48] =  0.0011332 ;  baseerror_fit_noweak[ M_GAMMA48] =    0.0002456 ;
  basevalue_fit_noweak[ M_GAMMA62] =  0.0894027 ;  baseerror_fit_noweak[ M_GAMMA62] =    0.0006513 ;
  basevalue_fit_noweak[ M_GAMMA70] =  0.0272508 ;  baseerror_fit_noweak[ M_GAMMA70] =    0.0007351 ;
  basevalue_fit_noweak[ M_GAMMA77] =  0.0008797 ;  baseerror_fit_noweak[ M_GAMMA77] =    0.0003577 ;
  basevalue_fit_noweak[ M_GAMMA78] =  0.0002219 ;  baseerror_fit_noweak[ M_GAMMA78] =    0.0000499 ;
  basevalue_fit_noweak[ M_GAMMA85] =  0.0033015 ;  baseerror_fit_noweak[ M_GAMMA85] =    0.0002251 ;
  basevalue_fit_noweak[ M_GAMMA89] =  0.0007313 ;  baseerror_fit_noweak[ M_GAMMA89] =    0.0001169 ;
  basevalue_fit_noweak[ M_GAMMA93] =  0.0015209 ;  baseerror_fit_noweak[ M_GAMMA93] =    0.0000756 ;
  basevalue_fit_noweak[ M_GAMMA94] =  0.0000563 ;  baseerror_fit_noweak[ M_GAMMA94] =    0.0000184 ;
  basevalue_fit_noweak[M_GAMMA104] =  0.0001794 ;  baseerror_fit_noweak[M_GAMMA104] =    0.0000263 ;
  basevalue_fit_noweak[M_GAMMA126] =  0.0017743 ;  baseerror_fit_noweak[M_GAMMA126] =    0.0002355 ;
  basevalue_fit_noweak[M_GAMMA128] =  0.0002675 ;  baseerror_fit_noweak[M_GAMMA128] =    0.0000633 ;
  basevalue_fit_noweak[M_GAMMA150] =  0.0200386 ;  baseerror_fit_noweak[M_GAMMA150] =    0.0006458 ;
  basevalue_fit_noweak[M_GAMMA152] =  0.0040246 ;  baseerror_fit_noweak[M_GAMMA152] =    0.0004193 ;
  basevalue_fit_noweak[M_GAMMA103] = 1-0.9991920;  baseerror_fit_noweak[M_GAMMA103] =    0.0000533 ;
  
  double basecorr_fit_noweak[31][31];
  basecorr_fit_noweak[  M_GAMMA3][  M_GAMMA5] = -0.1375455 ;
  basecorr_fit_noweak[  M_GAMMA3][  M_GAMMA9] = -0.0409309 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA10] =  0.0066298 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA14] = -0.1269672 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA16] =  0.0062131 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA20] = -0.0710701 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA23] = -0.0005722 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA27] = -0.0470787 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA28] =  0.0005206 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA30] = -0.0303932 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA35] = -0.0532583 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA37] = -0.0129175 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA40] = -0.0498093 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA42] = -0.0144886 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA47] = -0.0050634 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA48] = -0.0277819 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA62] = -0.0550534 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA70] = -0.0308252 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA77] =  0.0009626 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA78] =  0.0019603 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA85] =  0.0021900 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA89] =  0.0072195 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA93] = -0.0064268 ;
  basecorr_fit_noweak[  M_GAMMA3][ M_GAMMA94] =  0.0008653 ;
  basecorr_fit_noweak[  M_GAMMA3][M_GAMMA104] = -0.0101093 ;
  basecorr_fit_noweak[  M_GAMMA3][M_GAMMA126] = -0.0171050 ;
  basecorr_fit_noweak[  M_GAMMA3][M_GAMMA128] = -0.0039864 ;
  basecorr_fit_noweak[  M_GAMMA3][M_GAMMA150] = -0.0213169 ;
  basecorr_fit_noweak[  M_GAMMA3][M_GAMMA152] = -0.0134004 ;
  basecorr_fit_noweak[  M_GAMMA5][  M_GAMMA9] = -0.0458801 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA10] =  0.0103127 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA14] = -0.1458602 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA16] =  0.0098842 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA20] = -0.0771244 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA23] =  0.0003113 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA27] = -0.0300168 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA28] = -0.0009704 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA30] = -0.0207614 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA35] = -0.0525727 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA37] = -0.0133161 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA40] = -0.0494193 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA42] = -0.0151999 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA47] = -0.0049944 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA48] = -0.0273902 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA62] = -0.0692373 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA70] = -0.0340518 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA77] =  0.0026584 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA78] =  0.0027213 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA85] =  0.0063270 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA89] =  0.0095718 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA93] = -0.0071480 ;
  basecorr_fit_noweak[  M_GAMMA5][ M_GAMMA94] =  0.0011654 ;
  basecorr_fit_noweak[  M_GAMMA5][M_GAMMA104] = -0.0039989 ;
  basecorr_fit_noweak[  M_GAMMA5][M_GAMMA126] = -0.0171408 ;
  basecorr_fit_noweak[  M_GAMMA5][M_GAMMA128] = -0.0041310 ;
  basecorr_fit_noweak[  M_GAMMA5][M_GAMMA150] = -0.0218055 ;
  basecorr_fit_noweak[  M_GAMMA5][M_GAMMA152] = -0.0110980 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA10] = -0.2135785 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA14] = -0.1502548 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA16] = -0.0160496 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA20] = -0.1341099 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA23] = -0.0022510 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA27] = -0.1110654 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA28] =  0.0096363 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA30] = -0.1251348 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA35] = -0.0598506 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA37] = -0.0074284 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA40] = -0.0558674 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA42] = -0.0063364 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA47] = -0.0057008 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA48] = -0.0314613 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA62] =  0.0335497 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA70] = -0.0708101 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA77] = -0.0276017 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA78] =  0.0020583 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA85] = -0.0183637 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA89] =  0.0016199 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA93] = -0.0005955 ;
  basecorr_fit_noweak[  M_GAMMA9][ M_GAMMA94] =  0.0001686 ;
  basecorr_fit_noweak[  M_GAMMA9][M_GAMMA104] = -0.0077300 ;
  basecorr_fit_noweak[  M_GAMMA9][M_GAMMA126] = -0.0153904 ;
  basecorr_fit_noweak[  M_GAMMA9][M_GAMMA128] = -0.0025226 ;
  basecorr_fit_noweak[  M_GAMMA9][M_GAMMA150] = -0.0378133 ;
  basecorr_fit_noweak[  M_GAMMA9][M_GAMMA152] = -0.0506749 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA14] = -0.0184062 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA16] = -0.0403346 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA20] = -0.0160373 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA23] = -0.0327166 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA27] =  0.0114403 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA28] = -0.0315394 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA30] =  0.0227995 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA35] = -0.0341075 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA37] = -0.0131377 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA40] = -0.0318086 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA42] = -0.0160343 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA47] = -0.0032411 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA48] = -0.0177443 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA62] = -0.0436424 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA70] =  0.0272818 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA77] =  0.0095336 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA78] = -0.0058933 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA85] = -0.0221256 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA89] = -0.0122146 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA93] = -0.0112307 ;
  basecorr_fit_noweak[ M_GAMMA10][ M_GAMMA94] = -0.0016648 ;
  basecorr_fit_noweak[ M_GAMMA10][M_GAMMA104] =  0.0015731 ;
  basecorr_fit_noweak[ M_GAMMA10][M_GAMMA126] = -0.0206562 ;
  basecorr_fit_noweak[ M_GAMMA10][M_GAMMA128] = -0.0060360 ;
  basecorr_fit_noweak[ M_GAMMA10][M_GAMMA150] = -0.0004718 ;
  basecorr_fit_noweak[ M_GAMMA10][M_GAMMA152] =  0.0007048 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA16] = -0.1138841 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA20] = -0.4931930 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA23] = -0.0060023 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA27] =  0.0242035 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA28] =  0.0129630 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA30] = -0.0925669 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA35] = -0.0332043 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA37] =  0.0099148 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA40] = -0.0332821 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA42] =  0.0150951 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA47] = -0.0031362 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA48] = -0.0172832 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA62] = -0.0393727 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA70] = -0.1034158 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA77] = -0.0149910 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA78] =  0.0061271 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA85] =  0.0002894 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA89] =  0.0114898 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA93] = -0.0049796 ;
  basecorr_fit_noweak[ M_GAMMA14][ M_GAMMA94] =  0.0015052 ;
  basecorr_fit_noweak[ M_GAMMA14][M_GAMMA104] = -0.0120469 ;
  basecorr_fit_noweak[ M_GAMMA14][M_GAMMA126] = -0.0096431 ;
  basecorr_fit_noweak[ M_GAMMA14][M_GAMMA128] =  0.0018074 ;
  basecorr_fit_noweak[ M_GAMMA14][M_GAMMA150] = -0.0401986 ;
  basecorr_fit_noweak[ M_GAMMA14][M_GAMMA152] = -0.0272471 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA20] = -0.0106320 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA23] = -0.2009200 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA27] =  0.0161625 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA28] = -0.1825178 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA30] =  0.0011495 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA35] = -0.0080310 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA37] = -0.1060810 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA40] =  0.0068796 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA42] = -0.1413752 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA47] = -0.0009195 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA48] = -0.0050317 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA62] = -0.0231719 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA70] =  0.0195893 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA77] =  0.0049456 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA78] = -0.0045485 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA85] = -0.0191171 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA89] = -0.0067888 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA93] = -0.0078690 ;
  basecorr_fit_noweak[ M_GAMMA16][ M_GAMMA94] = -0.0013913 ;
  basecorr_fit_noweak[ M_GAMMA16][M_GAMMA104] =  0.0017075 ;
  basecorr_fit_noweak[ M_GAMMA16][M_GAMMA126] = -0.0134901 ;
  basecorr_fit_noweak[ M_GAMMA16][M_GAMMA128] = -0.0330138 ;
  basecorr_fit_noweak[ M_GAMMA16][M_GAMMA150] = -0.0003545 ;
  basecorr_fit_noweak[ M_GAMMA16][M_GAMMA152] = -0.0013770 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA23] = -0.0660365 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA27] = -0.3768302 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA28] = -0.0319754 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA30] =  0.0908876 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA35] = -0.0615241 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA37] =  0.0010814 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA40] = -0.0537453 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA42] =  0.0072589 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA47] = -0.0058985 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA48] = -0.0323627 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA62] = -0.1368772 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA70] =  0.0254756 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA77] =  0.0058270 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA78] = -0.0068256 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA85] = -0.0245444 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA89] = -0.0131239 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA93] = -0.0236344 ;
  basecorr_fit_noweak[ M_GAMMA20][ M_GAMMA94] = -0.0017569 ;
  basecorr_fit_noweak[ M_GAMMA20][M_GAMMA104] =  0.0078724 ;
  basecorr_fit_noweak[ M_GAMMA20][M_GAMMA126] = -0.0519010 ;
  basecorr_fit_noweak[ M_GAMMA20][M_GAMMA128] = -0.0044567 ;
  basecorr_fit_noweak[ M_GAMMA20][M_GAMMA150] = -0.0059933 ;
  basecorr_fit_noweak[ M_GAMMA20][M_GAMMA152] = -0.0149349 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA27] = -0.0031205 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA28] = -0.1548039 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA30] = -0.0124505 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA35] = -0.0075239 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA37] = -0.0853789 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA40] =  0.0046291 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA42] = -0.1136784 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA47] = -0.0008428 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA48] = -0.0046313 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA62] = -0.0087257 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA70] =  0.0063754 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA77] =  0.0013543 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA78] = -0.0030573 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA85] = -0.0139038 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA89] = -0.0039695 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA93] = -0.0047044 ;
  basecorr_fit_noweak[ M_GAMMA23][ M_GAMMA94] = -0.0009381 ;
  basecorr_fit_noweak[ M_GAMMA23][M_GAMMA104] =  0.0003121 ;
  basecorr_fit_noweak[ M_GAMMA23][M_GAMMA126] = -0.0130710 ;
  basecorr_fit_noweak[ M_GAMMA23][M_GAMMA128] = -0.0272362 ;
  basecorr_fit_noweak[ M_GAMMA23][M_GAMMA150] = -0.0035248 ;
  basecorr_fit_noweak[ M_GAMMA23][M_GAMMA152] = -0.0050888 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA28] = -0.0975019 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA30] = -0.4314314 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA35] = -0.0138092 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA37] =  0.0087486 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA40] = -0.0329880 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA42] =  0.0044338 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA47] = -0.0010637 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA48] = -0.0057009 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA62] = -0.0813948 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA70] = -0.0024948 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA77] =  0.0181677 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA78] = -0.0021348 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA85] =  0.0015014 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA89] = -0.0000567 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA93] = -0.0098909 ;
  basecorr_fit_noweak[ M_GAMMA27][ M_GAMMA94] = -0.0001166 ;
  basecorr_fit_noweak[ M_GAMMA27][M_GAMMA104] =  0.0019529 ;
  basecorr_fit_noweak[ M_GAMMA27][M_GAMMA126] =  0.0100700 ;
  basecorr_fit_noweak[ M_GAMMA27][M_GAMMA128] = -0.0069599 ;
  basecorr_fit_noweak[ M_GAMMA27][M_GAMMA150] = -0.0032814 ;
  basecorr_fit_noweak[ M_GAMMA27][M_GAMMA152] =  0.0222744 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA30] =  0.0180810 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA35] = -0.0064080 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA37] = -0.0795486 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA40] =  0.0001550 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA42] = -0.1080032 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA47] = -0.0006712 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA48] = -0.0037104 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA62] =  0.0014468 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA70] =  0.0036034 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA77] = -0.0014748 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA78] = -0.0026378 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA85] = -0.0124237 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA89] = -0.0028842 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA93] = -0.0030389 ;
  basecorr_fit_noweak[ M_GAMMA28][ M_GAMMA94] = -0.0008003 ;
  basecorr_fit_noweak[ M_GAMMA28][M_GAMMA104] = -0.0000432 ;
  basecorr_fit_noweak[ M_GAMMA28][M_GAMMA126] = -0.0149177 ;
  basecorr_fit_noweak[ M_GAMMA28][M_GAMMA128] = -0.0273404 ;
  basecorr_fit_noweak[ M_GAMMA28][M_GAMMA150] = -0.0040102 ;
  basecorr_fit_noweak[ M_GAMMA28][M_GAMMA152] = -0.0087108 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA35] =  0.0030453 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA37] = -0.0012365 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA40] =  0.0110919 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA42] =  0.0016452 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA47] =  0.0001944 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA48] =  0.0011856 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA62] = -0.0584001 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA70] =  0.0431002 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA77] =  0.0281856 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA78] = -0.0041580 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA85] = -0.0029793 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA89] = -0.0072461 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA93] = -0.0081091 ;
  basecorr_fit_noweak[ M_GAMMA30][ M_GAMMA94] = -0.0009096 ;
  basecorr_fit_noweak[ M_GAMMA30][M_GAMMA104] = -0.0079570 ;
  basecorr_fit_noweak[ M_GAMMA30][M_GAMMA126] = -0.0632695 ;
  basecorr_fit_noweak[ M_GAMMA30][M_GAMMA128] =  0.0013858 ;
  basecorr_fit_noweak[ M_GAMMA30][M_GAMMA150] =  0.0148542 ;
  basecorr_fit_noweak[ M_GAMMA30][M_GAMMA152] =  0.0246474 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA37] = -0.1295270 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA40] = -0.1513847 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA42] = -0.0131571 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA47] = -0.0257700 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA48] = -0.1413400 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA62] = -0.0514407 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA70] =  0.0304352 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA77] =  0.0094536 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA78] = -0.0091073 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA85] = -0.0400015 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA89] = -0.0195769 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA93] = -0.0168618 ;
  basecorr_fit_noweak[ M_GAMMA35][ M_GAMMA94] = -0.0025785 ;
  basecorr_fit_noweak[ M_GAMMA35][M_GAMMA104] = -0.0004164 ;
  basecorr_fit_noweak[ M_GAMMA35][M_GAMMA126] = -0.0416043 ;
  basecorr_fit_noweak[ M_GAMMA35][M_GAMMA128] = -0.0039590 ;
  basecorr_fit_noweak[ M_GAMMA35][M_GAMMA150] = -0.0117608 ;
  basecorr_fit_noweak[ M_GAMMA35][M_GAMMA152] = -0.0139481 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA40] =  0.0004063 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA42] = -0.1558062 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA47] = -0.0053200 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA48] = -0.0291754 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA62] = -0.0059302 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA70] =  0.0017720 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA77] =  0.0011580 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA78] = -0.0004209 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA85] = -0.0018668 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA89] =  0.0011695 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA93] = -0.0012344 ;
  basecorr_fit_noweak[ M_GAMMA37][ M_GAMMA94] = -0.0000855 ;
  basecorr_fit_noweak[ M_GAMMA37][M_GAMMA104] = -0.0004235 ;
  basecorr_fit_noweak[ M_GAMMA37][M_GAMMA126] = -0.0029112 ;
  basecorr_fit_noweak[ M_GAMMA37][M_GAMMA128] = -0.0150108 ;
  basecorr_fit_noweak[ M_GAMMA37][M_GAMMA150] = -0.0015269 ;
  basecorr_fit_noweak[ M_GAMMA37][M_GAMMA152] = -0.0010098 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA42] = -0.1990821 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA47] = -0.0245628 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA48] = -0.1347214 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA62] = -0.0482023 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA70] =  0.0295342 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA77] =  0.0087708 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA78] = -0.0088165 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA85] = -0.0389525 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA89] = -0.0192593 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA93] = -0.0161808 ;
  basecorr_fit_noweak[ M_GAMMA40][ M_GAMMA94] = -0.0025097 ;
  basecorr_fit_noweak[ M_GAMMA40][M_GAMMA104] = -0.0004031 ;
  basecorr_fit_noweak[ M_GAMMA40][M_GAMMA126] = -0.0405457 ;
  basecorr_fit_noweak[ M_GAMMA40][M_GAMMA128] = -0.0021808 ;
  basecorr_fit_noweak[ M_GAMMA40][M_GAMMA150] = -0.0112970 ;
  basecorr_fit_noweak[ M_GAMMA40][M_GAMMA152] = -0.0139509 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA47] = -0.0059114 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA48] = -0.0324191 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA62] = -0.0048917 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA70] =  0.0009090 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA77] =  0.0009562 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA78] = -0.0001101 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA85] = -0.0006079 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA89] =  0.0025269 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA93] = -0.0007777 ;
  basecorr_fit_noweak[ M_GAMMA42][ M_GAMMA94] =  0.0000099 ;
  basecorr_fit_noweak[ M_GAMMA42][M_GAMMA104] = -0.0005696 ;
  basecorr_fit_noweak[ M_GAMMA42][M_GAMMA126] = -0.0020004 ;
  basecorr_fit_noweak[ M_GAMMA42][M_GAMMA128] = -0.0200399 ;
  basecorr_fit_noweak[ M_GAMMA42][M_GAMMA150] = -0.0014773 ;
  basecorr_fit_noweak[ M_GAMMA42][M_GAMMA152] = -0.0008730 ;
  basecorr_fit_noweak[ M_GAMMA47][ M_GAMMA48] = -0.0286781 ;
  basecorr_fit_noweak[ M_GAMMA47][ M_GAMMA62] = -0.0048961 ;
  basecorr_fit_noweak[ M_GAMMA47][ M_GAMMA70] =  0.0028556 ;
  basecorr_fit_noweak[ M_GAMMA47][ M_GAMMA77] =  0.0009407 ;
  basecorr_fit_noweak[ M_GAMMA47][ M_GAMMA78] = -0.0008647 ;
  basecorr_fit_noweak[ M_GAMMA47][ M_GAMMA85] = -0.0037868 ;
  basecorr_fit_noweak[ M_GAMMA47][ M_GAMMA89] = -0.0018530 ;
  basecorr_fit_noweak[ M_GAMMA47][ M_GAMMA93] = -0.0015996 ;
  basecorr_fit_noweak[ M_GAMMA47][ M_GAMMA94] = -0.0002443 ;
  basecorr_fit_noweak[ M_GAMMA47][M_GAMMA104] = -0.0000382 ;
  basecorr_fit_noweak[ M_GAMMA47][M_GAMMA126] = -0.0039373 ;
  basecorr_fit_noweak[ M_GAMMA47][M_GAMMA128] = -0.0003925 ;
  basecorr_fit_noweak[ M_GAMMA47][M_GAMMA150] = -0.0011233 ;
  basecorr_fit_noweak[ M_GAMMA47][M_GAMMA152] = -0.0012634 ;
  basecorr_fit_noweak[ M_GAMMA48][ M_GAMMA62] = -0.0269857 ;
  basecorr_fit_noweak[ M_GAMMA48][ M_GAMMA70] =  0.0150874 ;
  basecorr_fit_noweak[ M_GAMMA48][ M_GAMMA77] =  0.0060754 ;
  basecorr_fit_noweak[ M_GAMMA48][ M_GAMMA78] = -0.0047942 ;
  basecorr_fit_noweak[ M_GAMMA48][ M_GAMMA85] = -0.0208045 ;
  basecorr_fit_noweak[ M_GAMMA48][ M_GAMMA89] = -0.0102198 ;
  basecorr_fit_noweak[ M_GAMMA48][ M_GAMMA93] = -0.0087995 ;
  basecorr_fit_noweak[ M_GAMMA48][ M_GAMMA94] = -0.0013474 ;
  basecorr_fit_noweak[ M_GAMMA48][M_GAMMA104] = -0.0001834 ;
  basecorr_fit_noweak[ M_GAMMA48][M_GAMMA126] = -0.0216158 ;
  basecorr_fit_noweak[ M_GAMMA48][M_GAMMA128] = -0.0021566 ;
  basecorr_fit_noweak[ M_GAMMA48][M_GAMMA150] = -0.0063567 ;
  basecorr_fit_noweak[ M_GAMMA48][M_GAMMA152] = -0.0057659 ;
  basecorr_fit_noweak[ M_GAMMA62][ M_GAMMA70] = -0.2419874 ;
  basecorr_fit_noweak[ M_GAMMA62][ M_GAMMA77] =  0.0008315 ;
  basecorr_fit_noweak[ M_GAMMA62][ M_GAMMA78] = -0.0129241 ;
  basecorr_fit_noweak[ M_GAMMA62][ M_GAMMA85] = -0.1271655 ;
  basecorr_fit_noweak[ M_GAMMA62][ M_GAMMA89] = -0.0501206 ;
  basecorr_fit_noweak[ M_GAMMA62][ M_GAMMA93] =  0.0930862 ;
  basecorr_fit_noweak[ M_GAMMA62][ M_GAMMA94] = -0.0065000 ;
  basecorr_fit_noweak[ M_GAMMA62][M_GAMMA104] = -0.0089208 ;
  basecorr_fit_noweak[ M_GAMMA62][M_GAMMA126] = -0.0222430 ;
  basecorr_fit_noweak[ M_GAMMA62][M_GAMMA128] = -0.0036854 ;
  basecorr_fit_noweak[ M_GAMMA62][M_GAMMA150] = -0.1076877 ;
  basecorr_fit_noweak[ M_GAMMA62][M_GAMMA152] = -0.0149608 ;
  basecorr_fit_noweak[ M_GAMMA70][ M_GAMMA77] = -0.0788355 ;
  basecorr_fit_noweak[ M_GAMMA70][ M_GAMMA78] = -0.0170531 ;
  basecorr_fit_noweak[ M_GAMMA70][ M_GAMMA85] = -0.0329766 ;
  basecorr_fit_noweak[ M_GAMMA70][ M_GAMMA89] = -0.0746097 ;
  basecorr_fit_noweak[ M_GAMMA70][ M_GAMMA93] = -0.0403534 ;
  basecorr_fit_noweak[ M_GAMMA70][ M_GAMMA94] = -0.0095664 ;
  basecorr_fit_noweak[ M_GAMMA70][M_GAMMA104] =  0.0018017 ;
  basecorr_fit_noweak[ M_GAMMA70][M_GAMMA126] =  0.0108381 ;
  basecorr_fit_noweak[ M_GAMMA70][M_GAMMA128] =  0.0014791 ;
  basecorr_fit_noweak[ M_GAMMA70][M_GAMMA150] = -0.6045022 ;
  basecorr_fit_noweak[ M_GAMMA70][M_GAMMA152] = -0.1098082 ;
  basecorr_fit_noweak[ M_GAMMA77][ M_GAMMA78] = -0.0061227 ;
  basecorr_fit_noweak[ M_GAMMA77][ M_GAMMA85] =  0.0037793 ;
  basecorr_fit_noweak[ M_GAMMA77][ M_GAMMA89] = -0.0050472 ;
  basecorr_fit_noweak[ M_GAMMA77][ M_GAMMA93] =  0.0010257 ;
  basecorr_fit_noweak[ M_GAMMA77][ M_GAMMA94] = -0.0006444 ;
  basecorr_fit_noweak[ M_GAMMA77][M_GAMMA104] =  0.0039785 ;
  basecorr_fit_noweak[ M_GAMMA77][M_GAMMA126] = -0.1499521 ;
  basecorr_fit_noweak[ M_GAMMA77][M_GAMMA128] =  0.0002768 ;
  basecorr_fit_noweak[ M_GAMMA77][M_GAMMA150] = -0.0266080 ;
  basecorr_fit_noweak[ M_GAMMA77][M_GAMMA152] = -0.6193242 ;
  basecorr_fit_noweak[ M_GAMMA78][ M_GAMMA85] = -0.0082154 ;
  basecorr_fit_noweak[ M_GAMMA78][ M_GAMMA89] = -0.0074375 ;
  basecorr_fit_noweak[ M_GAMMA78][ M_GAMMA93] = -0.0037738 ;
  basecorr_fit_noweak[ M_GAMMA78][ M_GAMMA94] = -0.0009663 ;
  basecorr_fit_noweak[ M_GAMMA78][M_GAMMA104] =  0.0008896 ;
  basecorr_fit_noweak[ M_GAMMA78][M_GAMMA126] = -0.0047484 ;
  basecorr_fit_noweak[ M_GAMMA78][M_GAMMA128] = -0.0006580 ;
  basecorr_fit_noweak[ M_GAMMA78][M_GAMMA150] = -0.0085456 ;
  basecorr_fit_noweak[ M_GAMMA78][M_GAMMA152] = -0.0106645 ;
  basecorr_fit_noweak[ M_GAMMA85][ M_GAMMA89] = -0.0235444 ;
  basecorr_fit_noweak[ M_GAMMA85][ M_GAMMA93] = -0.0978312 ;
  basecorr_fit_noweak[ M_GAMMA85][ M_GAMMA94] = -0.0030740 ;
  basecorr_fit_noweak[ M_GAMMA85][M_GAMMA104] =  0.0003622 ;
  basecorr_fit_noweak[ M_GAMMA85][M_GAMMA126] = -0.0220167 ;
  basecorr_fit_noweak[ M_GAMMA85][M_GAMMA128] = -0.0030395 ;
  basecorr_fit_noweak[ M_GAMMA85][M_GAMMA150] = -0.0235771 ;
  basecorr_fit_noweak[ M_GAMMA85][M_GAMMA152] = -0.0079976 ;
  basecorr_fit_noweak[ M_GAMMA89][ M_GAMMA93] = -0.0123680 ;
  basecorr_fit_noweak[ M_GAMMA89][ M_GAMMA94] = -0.0314216 ;
  basecorr_fit_noweak[ M_GAMMA89][M_GAMMA104] =  0.0013501 ;
  basecorr_fit_noweak[ M_GAMMA89][M_GAMMA126] = -0.0102996 ;
  basecorr_fit_noweak[ M_GAMMA89][M_GAMMA128] = -0.1236722 ;
  basecorr_fit_noweak[ M_GAMMA89][M_GAMMA150] =  0.0216906 ;
  basecorr_fit_noweak[ M_GAMMA89][M_GAMMA152] = -0.0125103 ;
  basecorr_fit_noweak[ M_GAMMA93][ M_GAMMA94] = -0.0016094 ;
  basecorr_fit_noweak[ M_GAMMA93][M_GAMMA104] = -0.0010309 ;
  basecorr_fit_noweak[ M_GAMMA93][M_GAMMA126] = -0.0085163 ;
  basecorr_fit_noweak[ M_GAMMA93][M_GAMMA128] = -0.0012535 ;
  basecorr_fit_noweak[ M_GAMMA93][M_GAMMA150] = -0.0175604 ;
  basecorr_fit_noweak[ M_GAMMA93][M_GAMMA152] = -0.0040371 ;
  basecorr_fit_noweak[ M_GAMMA94][M_GAMMA104] =  0.0001716 ;
  basecorr_fit_noweak[ M_GAMMA94][M_GAMMA126] = -0.0013749 ;
  basecorr_fit_noweak[ M_GAMMA94][M_GAMMA128] = -0.0001837 ;
  basecorr_fit_noweak[ M_GAMMA94][M_GAMMA150] =  0.0027704 ;
  basecorr_fit_noweak[ M_GAMMA94][M_GAMMA152] = -0.0016329 ;
  basecorr_fit_noweak[M_GAMMA104][M_GAMMA126] =  0.0004188 ;
  basecorr_fit_noweak[M_GAMMA104][M_GAMMA128] = -0.0001202 ;
  basecorr_fit_noweak[M_GAMMA104][M_GAMMA150] =  0.0006115 ;
  basecorr_fit_noweak[M_GAMMA104][M_GAMMA152] =  0.0050260 ;
  basecorr_fit_noweak[M_GAMMA126][M_GAMMA128] = -0.0032529 ;
  basecorr_fit_noweak[M_GAMMA126][M_GAMMA150] = -0.0072365 ;
  basecorr_fit_noweak[M_GAMMA126][M_GAMMA152] = -0.0102868 ;
  basecorr_fit_noweak[M_GAMMA128][M_GAMMA150] = -0.0011155 ;
  basecorr_fit_noweak[M_GAMMA128][M_GAMMA152] = -0.0015893 ;
  basecorr_fit_noweak[M_GAMMA150][M_GAMMA152] = -0.0379326 ;

  for (i=0;i<31;++i) for (j=0;j<31;++j) if (i==j) basecorr_fit_noweak[i][j]=1;
  for (i=0;i<30;++i) for (j=0;j<30;++j) if (i>j)  basecorr_fit_noweak[i][j]=basecorr_fit_noweak[j][i];
  for (i=0;i<31;++i) {
    cout << "basecorr["<<i<<"]=";
    for (j=0;j<31;++j) {
      cout << Form("%8.4g ",basecorr_fit_noweak[i][j]);
      if (j%10==9) cout << endl;
    }
    cout << endl; 
  }
  double basecov_fit_noweak[31][31];
  for (i=0;i<30;++i){
    for (j=0;j<30;++j){
      basecov_fit_noweak[i][j] = basecorr_fit_noweak[i][j]*baseerror_fit_noweak[i]*baseerror_fit_noweak[j];
    }
  }
  for (i=0;i<30;++i){
    basecov_fit_noweak[i][30] = 0;
    for (int j=0;j<30;++j) {
      basecov_fit_noweak[i][30] -= basecov_fit_noweak[i][j];
    }
  }
  for (i=0;i<30;++i){
    basecov_fit_noweak[30][i] = basecov_fit_noweak[i][30];
  }
  basecov_fit_noweak[30][30] = baseerror_fit_noweak[30]*baseerror_fit_noweak[30];
  for (i=0;i<31;++i) {
    cout << "basecov["<<i<<"]=";
    for (j=0;j<31;++j) {
      cout << Form("%8.4g ",basecov_fit_noweak[i][j]);
      if (j%10==9) cout << endl;
    }
    cout << endl; 
  }
  for (i=0;i<30;++i){
    basecorr_fit_noweak[i][30] = basecov_fit_noweak[i][30] / (baseerror_fit_noweak[i]*baseerror_fit_noweak[30]);
    basecorr_fit_noweak[30][i] = basecov_fit_noweak[i][30] / (baseerror_fit_noweak[i]*baseerror_fit_noweak[30]);
  }
  //
  vector<double> FitValue_part_re_noweak[nmeas];
  double FitValue_num_re_noweak[nmeas];
  double FitValue_den_re_noweak[nmeas];
  double FitValue_re_noweak[nmeas];
  for (i=0;i<nmeas;++i) {
    vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
    inode=it-nodename.begin();
    FitValue_num_re_noweak[i]=0; // sum_j=1,n alpha_j P_j
    if (node_num_npar[inode]>0) {
      for (ipar=0;ipar<node_num_npar[inode];++ipar){
	int parm=node_num_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	FitValue_num_re_noweak[i]+=(node_num_coef[inode].at(ipar))*(basevalue_fit_noweak[quan-1]);
      }
    }
    FitValue_den_re_noweak[i]=0; // sum_k=1,m beta_k P_k
    if (node_den_npar[inode]>0) {
      for (ipar=0;ipar<node_den_npar[inode];++ipar){
	int parm=node_den_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	FitValue_den_re_noweak[i]+=(node_den_coef[inode].at(ipar))*(basevalue_fit_noweak[quan-1]);
      }
    }
    FitValue_re_noweak[i] = (FitValue_den_re_noweak[i]!=0) ? FitValue_num_re_noweak[i]/FitValue_den_re_noweak[i] : FitValue_num_re_noweak[i];
    //
    for (ipar=0;ipar<node_parm[inode].size();++ipar){
      int parm=node_parm[inode].at(ipar);
      vector<int>::iterator it_num=find(node_num_parm[inode].begin(),node_num_parm[inode].end(),parm); bool is_in_num = it_num != node_num_parm[inode].end();
      vector<int>::iterator it_den=find(node_den_parm[inode].begin(),node_den_parm[inode].end(),parm); bool is_in_den = it_den != node_den_parm[inode].end();
      double partial=0;
      if (node_den_npar[inode]==0 && is_in_num) {//case 1
	partial = node_num_coef[inode].at(it_num - node_num_parm[inode].begin());
      } else if ( is_in_num && !is_in_den) { // case 2a
	partial = (1./FitValue_den_re_noweak[i]) * (node_num_coef[inode].at(it_num - node_num_parm[inode].begin()));
      } else if (!is_in_num &&  is_in_den) { // case 2b
	partial = -1. * (FitValue_num_re_noweak[i]/(FitValue_den_re_noweak[i]*FitValue_den_re_noweak[i])) * (node_den_coef[inode].at(it_den - node_den_parm[inode].begin()));
      } else if ( is_in_num &&  is_in_den) { // case 2c
	partial = (1./FitValue_den_re_noweak[i]) * (node_num_coef[inode].at(it_num - node_num_parm[inode].begin())) -1. * (FitValue_num_re_noweak[i]/(FitValue_den_re_noweak[i]*FitValue_den_re_noweak[i])) * (node_den_coef[inode].at(it_den - node_den_parm[inode].begin()));
      }
      FitValue_part_re_noweak[i].push_back(partial); // <--
    }
  }
  //
  double FitError_re_noweak[nmeas];
  TMatrixD FitErrorMatrix_re_noweak(nmeas,nmeas);
  for (i=0;i<nmeas;++i) {
    vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
    inode=it-nodename.begin();
    for (j=0;j<nmeas;++j) {
      vector<string>::iterator jt=find(nodename.begin(),nodename.end(),measnodename[j]);      
      jnode=jt-nodename.begin();
      FitErrorMatrix_re_noweak[i][j] = 0;
      for (ipar = 0; ipar < node_parm[inode].size(); ++ipar) {
	int iquan=node_quan[inode].at(ipar);
        //double ipartial=node_part[inode].at(ipar);
	double ipartial=FitValue_part_re_noweak[i].at(ipar);
	for (jpar = 0; jpar < node_parm[jnode].size(); ++jpar) {
	  int jquan=node_quan[jnode].at(jpar);
	  //          double jpartial=node_part[jnode].at(jpar);
	  double jpartial=FitValue_part_re_noweak[j].at(jpar);
//	  cout << "i = " << i << " j = " << j << " ipartial = " << ipartial << " jpartial = " << jpartial << " iquan = " << iquan << " jquan = " << jquan << " "
//	       << "basefiterror_i = " << baseerror_fit[iquan-1] << " " 
//	       << "basefiterror_j = " << baseerror_fit[jquan-1] << " "
//	       << "basefitcorr_ij = " << basecorr_fit[iquan-1][jquan-1]
//	       << endl;
	  FitErrorMatrix_re_noweak[i][j] += ipartial * jpartial *  baseerror_fit_noweak[iquan-1] * baseerror_fit_noweak[jquan-1] * basecorr_fit_noweak[iquan-1][jquan-1];
	}
      }
    }
    //
    FitError_re_noweak[i] = TMath::Sqrt(FitErrorMatrix_re_noweak[i][i]);
  }
  //
  // Cross-check understanding of chisquare and pullaverage in the input file
  //
  //
  double nsig_fit[200];
  bool weak_re[200];
  for (i=0;i<nmeas;++i){
    if (n_nodegroup[nodegroup_all[i]] > 0 ) {
      nsig_fit[i] = measerror[i]/((sqrt(n_nodegroup[nodegroup_all[i]]))*FitError_re[i]);
    }else{
      nsig_fit[i]=0;
    }
    weak_re[i] = (icorrj[i].size()==1) && (nsig_fit[i] > 3.); // only for uncorrelated measurements
  }
  //
  double nsig_fit_noweak[200];
  bool weak_re_noweak[200];
  for (i=0;i<nmeas;++i){
    if (n_nodegroup[nodegroup_all[i]] > 0 ) {
      nsig_fit_noweak[i] = measerror[i]/((sqrt(n_nodegroup[nodegroup_all[i]]))*FitError_re_noweak[i]);
    }else{
      nsig_fit_noweak[i]=0;
    }
    weak_re_noweak[i] = (icorrj[i].size()==1) && (nsig_fit_noweak[i] > 3.); // only for uncorrelated measurements
  }
  //
  int im,jm;
  vector<TMatrixD> VectorOfMeasScaledErrorMatrix;
  vector<double> VectorOfPullMag2;
  vector<double> VectorOfPullMag;
  vector<double> VectorOfChi2;
  for (int ij=0;ij<ncorrij;++ij) {
    int nij=veccorrij[ij].size();
    TMatrixD ThisFitErrorMatrix(nij,nij);
    TMatrixD ThisMeasErrorMatrix(nij,nij);
    for (i=0;i<nij;++i){
      im=veccorrij[ij][i];
      vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[im]);      
      inode=it-nodename.begin();
      cout << " ij = " << ij << " i = " << i << " im = " << im << " " << measnodename[im] 
	   << " measeror = " << measerror[im] << " inode = " << inode << endl;
      for (j=0;j<nij;++j){
	jm=veccorrij[ij][j];
	vector<string>::iterator jt=find(nodename.begin(),nodename.end(),measnodename[jm]);      
	jnode=jt-nodename.begin();
	cout << " ij = " << ij << " j = " << j << " jm = " << jm << " " << measnodename[jm] 
	     << " measerror = " << measerror[jm] << " jnode = " << jnode 
	     << " corrmat = " << corrmat[im][jm] << endl;
	ThisMeasErrorMatrix[i][j] = (i==j) ? measerror[im]*measerror[im] : corrmat[im][jm] * measerror[im] * measerror[jm];
	ThisFitErrorMatrix[i][j]= FitErrorMatrix_re_noweak[im][jm];
      }
    }
    for (i=0;i<nij;++i) {
      cout << "FitErrorMatrix["<<i<<"] = ";
      for (j=0;j<nij;++j) {
	cout << Form("%8.4g ",ThisFitErrorMatrix[i][j]);
      }
      cout << endl; 
    }
    for (i=0;i<nij;++i) {
      cout << "MeasErrorMatrix["<<i<<"] = ";
      for (j=0;j<nij;++j) {
	cout << Form("%8.4g ",ThisMeasErrorMatrix[i][j]);
      }
      cout << endl; 
    }
    TMatrixD ThisInvMeasErrorMatrix = ThisMeasErrorMatrix;
    double thisdeterminant;
    ThisInvMeasErrorMatrix.Invert(&thisdeterminant);
    cout << " thisdeterminant = " << thisdeterminant << endl;
    for (i=0;i<nij;++i) {
      cout << "InvMeasErrorMatrix["<<i<<"] = ";
      for (j=0;j<nij;++j) {
	cout << Form("%8.4g ",ThisInvMeasErrorMatrix[i][j]);
      }
      cout << endl; 
    }
    double thischi2=0;
    for (i=0;i<nij;++i) {
      im=veccorrij[ij][i];
      for (j=0;j<nij;++j) {
	jm=veccorrij[ij][j];
	thischi2+=(measvalue[im]-FitValue_re_noweak[im])*ThisInvMeasErrorMatrix[i][j]*(measvalue[jm]-FitValue_re_noweak[jm]);
      }
    }
    thischi2/=nij;
    cout << "ThisChisquared = " << thischi2 << endl;
    VectorOfChi2.push_back(thischi2);
    // CMatrix = (MeasErrorMatrix - FitErrorMatrix)^{-1}
    TMatrixD InvCMatrix(nij,nij);
    for (i=0;i<nij;++i) {
      for (j=0;j<nij;++j) {
	InvCMatrix[i][j] = ThisMeasErrorMatrix[i][j] - ThisFitErrorMatrix[i][j];
      }
    }
    for (i=0;i<nij;++i) {
      cout << "InvCMatrix["<<i<<"] = ";
      for (j=0;j<nij;++j) {
	cout << Form("%8.4g ",InvCMatrix[i][j]);
      }
      cout << endl; 
    }
    TMatrixD CMatrix = InvCMatrix;
    Double_t c_det;
    CMatrix.Invert(&c_det);
    cout << " c_det = " << c_det << endl;
    for (i=0;i<nij;++i) {
      cout << "CMatrix["<<i<<"] = ";
      for (j=0;j<nij;++j) {
	cout << Form("%8.4g ",CMatrix[i][j]);
      }
      cout << endl; 
    }
    //
    TVectorD EigenVal;
    TMatrixD EigenVecT = CMatrix.EigenVectors(EigenVal);
    TMatrixD EigenVec(nij,nij); EigenVec.Transpose(EigenVecT);
    for (i=0;i<nij;++i) { 
      cout << "EigenVal["<<i<<"] = " << EigenVal[i] << " ";
      cout << "EigenVec["<<i<<"] : ";
      for (j=0;j<nij;++j) {
	cout << Form("%8.4g ",EigenVec[i][j]);
      }
      cout << endl;
    }
    //
    TMatrixD Diagonal(nij,nij);
    TMatrixD Diagonal_sqrt(nij,nij);
    for (i=0;i<nij;++i) {
      for (j=0;j<nij;++j) {
	if (i==j) {
	  Diagonal(i,j) = EigenVal[i];
	  Diagonal_sqrt(i,j) = TMath::Sqrt(EigenVal[i]);
	} else {
	  Diagonal(i,j) = 0;
	  Diagonal_sqrt(i,j) = 0;
	}
      }
    }
    //
    TMatrixD QMatrix = EigenVecT * Diagonal_sqrt * EigenVec; // Q = sqrt(C)
    for (i=0;i<nij;++i) {
      cout << "QMatrix["<<i<<"] = ";
      for (j=0;j<nij;++j) {
	cout << Form("%8.4g ",QMatrix[i][j]);
      }
      cout << endl; 
    }
    double FMinusM[nij];
    for (i=0;i<nij;++i) {
      im=veccorrij[ij][i];
      //FMinusM[i]=FitValue_re[im]-measvalue[im]; 
      FMinusM[i]=FitValue_re_noweak[im]-measvalue[im]; 
      cout << "i = " << i 
	   << " measvalue = " << measvalue[im] 
	   << " FitValue_re = " << FitValue_re[im] 
	   << " FitValue_re_noweak = " << FitValue_re_noweak[im] 
	   << " FMinusM = " << FMinusM[i] << endl;
    }
    double PullMag2=0;
    double PullVector[nij];
    for (i=0;i<nij;++i) {
      PullVector[i] = 0;
      for (j=0;j<nij;++j) {
	PullVector[i] += QMatrix[i][j] * FMinusM[j];
      }
      //      if (PullVector[i] < 0.0) PullVector[i]*=-1;
      PullMag2 += PullVector[i]*PullVector[i];
      cout << "PullVector[i] = " << PullVector[i] << " ";
    }
    cout << endl;
    VectorOfPullMag2.push_back(PullMag2);
    double PullMag = TMath::Sqrt(PullMag2);
    cout << "PullMag = " << PullMag << endl;
    VectorOfPullMag.push_back(PullMag);
    double UnitPullVector[nij];
    for (i=0;i<nij;++i) {
      UnitPullVector[i] = PullVector[i]/PullMag;
    }
    TMatrixD MeasCorrMatrix(nij,nij);
    for (i=0;i<nij;++i) {
      for (j=0;j<nij;++j) {
	if (i==j) {
	  MeasCorrMatrix[i][j] = 1; // by construction diagonal elements of corrmat has not been filled
	}else {
	  im=veccorrij[ij][i];
	  jm=veccorrij[ij][j];
	  MeasCorrMatrix[i][j]=corrmat[im][jm];
	}
      }
    }
    for (i=0;i<nij;++i) {
      cout << "MeasCorrMatrix["<<i<<"] = ";
      for (j=0;j<nij;++j) {
	cout << Form("%8.4g ",MeasCorrMatrix[i][j]);
      }
      cout << endl; 
    }
    double CM=0;
    for (i=0;i<nij;++i) {
      for (j=0;j<nij;++j) {
	CM += UnitPullVector[i]*MeasCorrMatrix[i][j]*UnitPullVector[j];
      }
    }
    double SFactor = (PullMag>1) ? (PullMag2 - 1) * CM : 0;
    cout << "SFactor = " << SFactor << endl;
    TMatrixD ThisMeasScaledErrorMatrix(nij,nij);
    for (i=0;i<nij;++i) {
      im=veccorrij[ij][i];
      for (j=0;j<nij;++j) {
	jm=veccorrij[ij][j];
	ThisMeasScaledErrorMatrix[i][j] = ThisMeasErrorMatrix[i][j] + UnitPullVector[i] * measerror[im] * UnitPullVector[j] * measerror[jm] * SFactor;
      }
    }
    for (i=0;i<nij;++i) {
      cout << "ThisMeasScaledErrorMatrix["<<i<<"] = ";
      for (j=0;j<nij;++j) {
	cout << Form("%8.4g ",ThisMeasScaledErrorMatrix[i][j]);
      }
      cout << endl; 
    }
    VectorOfMeasScaledErrorMatrix.push_back(ThisMeasScaledErrorMatrix);
  }
  //
  cout << "VectorOfPullMag2.size() = " << VectorOfPullMag2.size() << endl;
  cout << "VectorOfPullMag.size() = " << VectorOfPullMag.size() << endl;
  cout << "VectorOfChi2.size() = " << VectorOfChi2.size() << endl;
  for (int ij=0;ij<ncorrij;++ij){
    cout << "ij = " << ij << " VectorOfChi2 = " << VectorOfChi2[ij] << " VectorOfPullMag2 = " << VectorOfPullMag2[ij] << " VectorOfPullMag = " << VectorOfPullMag[ij] << endl;
  }
  //
  double chisquare_tot=0;
  double chisquare_re[200];
  double pullsquare_re[200];
  for (i=0;i<nmeas;++i){
    if ( icorrj[i].size() > 1 ) { // first find which cycle this correlated measurement belongs to, then just fetch the result
      chisquare_re[i]  = VectorOfChi2[ncycle[i]]; 
      pullsquare_re[i] = VectorOfPullMag2[ncycle[i]];
    } else {
      chisquare_re[i]  = TMath::Power(((measvalue[i]-FitValue_re_noweak[i])/measerror[i]),2);
      pullsquare_re[i] = TMath::Power((measvalue[i]-FitValue_re_noweak[i]),2)/(TMath::Power(measerror[i],2) - TMath::Power(FitError_re_noweak[i],2));
    }
    cout << "i = " << i << " ncycle[i] = " << ncycle[i] << " pullsquare_re[i] = " << pullsquare_re[i] << " chisquare_re[i] = " << chisquare_re[i] << endl;
    chisquare_tot+=chisquare_re[i];
  }
  cout << "chisquare_tot = " << chisquare_tot << endl;
  //
  double pullsq_pernode[200];
  double pullav_pernode[200];
  for (inode=0;inode<=newnode_all;++inode){
    pullsq_pernode[inode]=0;
    for (i=0;i<nmeas;++i){
      if (nodegroup_all[i]==inode){
	if (weak_re_noweak[i]==0) {
	  pullsq_pernode[inode]+=pullsquare_re[i];
	}
	cout << "i+1 = " << i+1 << " inode = " << inode 
	     << " n_nodegroup[inode] = " << n_nodegroup[inode] << " pullsq_pernode[inode] = " << pullsq_pernode[inode] << " weak_re_noweak = " << weak_re_noweak[i] << endl;
      }
    }
    pullav_pernode[inode]=(n_nodegroup[inode]>0) ? sqrt(pullsq_pernode[inode]/n_nodegroup[inode]) : 0;
    cout << "inode = " << inode << " n_nodegroup = " << n_nodegroup[inode] << " pullsq_pernode = " << pullsq_pernode[inode] << " pullav_pernode = " << pullav_pernode[inode] << endl;
  }
  double pullav_re[200];
  for (i=0;i<nmeas;++i){
    if (weak_re_noweak[i]==0) {
      pullav_re[i]=pullav_pernode[nodegroup_all[i]];
    }else{
      pullav_re[i]=0;
    }
    cout << "i+1 = " << i+1 << " node = " << nodegroup_all[i] << " corr = " << icorrj[i].size() << " n = " << n_nodegroup[nodegroup_all[i]] << " meas  = " << measvalue[i] << " +- " << measerror[i] << " fit = " << FitValue_re_noweak[i] << " +- " << FitError_re_noweak[i] << " pullsq = " << pullsquare_re[i] << " pullav = " << pullav_re[i] << " nsig_fit = " << nsig_fit[i] << " nsig_fit_noweak = " << nsig_fit_noweak[i] << " weak_re = " << weak_re[i]  << " weak_re_noweak = " << weak_re_noweak[i]  << endl;
  }
  //
  chisquare_tot=0;
  for (i=0; i<nmeas; ++i){
    chisquare_tot+=chisquare_re[i];
    cout << "Summary: i+1 = " << i+1 << " icorrj[i] = " << icorrj[i].size() << " chi2_re = " << chisquare_re[i] << " chi2sum = " << chisquare_tot << " pull_re = " << pullav_re[i] <<  " weak_re = " << weak_re[i] << " fit/err = " << FitValue_re_noweak[i]/FitError_re_noweak[i] << " meas/err = " << measvalue[i]/measerror[i] << endl;
  } 
  //
  FILE *scaled_measfile[2];
  for (int p=0;p<1;++p){
    if (p==0) scaled_measfile[0]=fopen(Form("combos_measurements_scaled_%s%s.input",sconstrain.data(),salephhcorr.data()),"w");
    if (p==1) scaled_measfile[1]=fopen(Form("alucomb_measurements_scaled_%s%s.input",sconstrain.data(),salephhcorr.data()),"w");
    iimeas=0;
    for (int i=0;i<nmeas;++i){
      if (weak_re[i]==1) continue;
      ++iimeas;
      //      cout << "i = " << i << endl;
      fprintf (scaled_measfile[p], "* IMEAS = %d \n",    iimeas);
      fprintf (scaled_measfile[p], "* GAMMANAME = %s \n", gammaname[i].data());
      fprintf (scaled_measfile[p], "* DECAYNAME = %s \n", meastitle[i].data());
      vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
      inode=it-nodename.begin();
      fprintf (scaled_measfile[p], "* NODENAME = %s found at inode+1 = %d with NODENAME[inode] = %s has %d, %d, %d base quantities in numerator, denominator and both [excluding overlap] \n", measnodename[i].data(), inode+1, nodename[inode].data(),node_num_npar[inode], node_den_npar[inode],node_parm[inode].size());
      for (ipar=0;ipar<node_num_parm[inode].size();++ipar){
	int parm=node_num_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	fprintf (scaled_measfile[p],"*                numerator of inode+1 = %d has gamma = %d parm = %d quan = %d title = %s seed = %f coef = %f\n",inode+1, basegamma[quan-1], parm, quan, basetitle[quan-1].data(), baseseed[quan-1], node_num_coef[inode].at(ipar));
      }
      for (ipar=0;ipar<node_den_parm[inode].size();++ipar){
	int parm=node_den_parm[inode].at(ipar);
	vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	int quan=ibase-baseparm.begin()+1;
	fprintf (scaled_measfile[p],"*              denominator of inode+1 = %d has gamma = %d parm = %d quan = %d title = %s seed = %f coef = %f\n",inode+1, basegamma[quan-1], parm, quan, basetitle[quan-1].data(), baseseed[quan-1], node_den_coef[inode].at(ipar));
      }
      fprintf (scaled_measfile[p], "*  first quantity measured by inode+1 = %d has gamma = %d parm = %d quan = %d title = %s\n",inode+1,basegamma[first_quan[inode]-1],baseparm[first_quan[inode]-1],first_quan[inode],basetitle[first_quan[inode]-1].data());
      //
      fprintf (scaled_measfile[p], "\nBEGIN %s Gamma%s pub.%s.%s \n\n", expname[i].data(), gammaname[i].data(), author[i].data(), year[i].data());
      if (p==0) {//COMBOS
	if (// SPECIAL CASE [because these nodes contain Gamma103 [used to express unitarity constraint]
	    (inode+1)==80 || // NODE = 79 NAME = S035R33 GAMMA = 102
	    (inode+1)==82) { // NODE = 81 NAME = S035R38 GAMMA = 103
	  fprintf (scaled_measfile[p], "MEASUREMENT  m_Gamma%d statistical systematic \n",3);
	  fprintf (scaled_measfile[p], "DATA         m_Gamma%d statistical systematic \n",3);
	}else{
	  fprintf (scaled_measfile[p], "MEASUREMENT  m_Gamma%d statistical systematic \n",basegamma[first_quan[inode]-1]);
	  fprintf (scaled_measfile[p], "DATA         m_Gamma%d statistical systematic \n",basegamma[first_quan[inode]-1]);
	}
      }else if (p==1) {//ALUCOMB
	fprintf (scaled_measfile[p], "MEASUREMENT  m_Gamma%s statistical systematic \n",gammaname[i].data());
	fprintf (scaled_measfile[p], "DATA         m_Gamma%s statistical systematic \n",gammaname[i].data());
      }
      if (icorrj[i].size() > 1 && VectorOfPullMag[ncycle[i]]>1) {
	double temperr2=measerror[i]*measerror[i]; // over-written below
	for (int itemp=0; itemp < veccorrij[ncycle[i]].size(); ++itemp) {
	  if (i == veccorrij[ncycle[i]][itemp]) {
	    temperr2=((TMatrixD)(VectorOfMeasScaledErrorMatrix[ncycle[i]]))(itemp,itemp);
	    break;
	  }
	}
	//	cout << " i+1 = " << i+1 << " temperr2 = " << temperr2 << endl;
	//fprintf (scaled_measfile[p], "             %10.5g %10.5g  0 \n",measvalue[i],measerror[i]);
	fprintf (scaled_measfile[p], "             %10.5g %10.5g  0 \n",measvalue[i],TMath::Sqrt( temperr2 ) );
      }else if (icorrj[i].size()==1 && pullav_re[i]>1 ) {
	//fprintf (scaled_measfile[p], "             %10.5g %10.5g  0 \n",measvalue[i],measerror[i]);
	fprintf (scaled_measfile[p], "             %10.5g %10.5g  0 \n",measvalue[i],measerror[i]*pullav_re[i]);
      }else{
	fprintf (scaled_measfile[p], "             %10.5g %10.5g  0 \n",measvalue[i],measerror[i]);
      }
      bool firstcorr=true;
      jjmeas=0;
      for (int j=0;j<nmeas;++j) {
	if (weak_re[j]==1) continue;
	++jjmeas;
	double tempcorrij=corrmat[i][j]; // over-written below
	if (icorrj[i].size() > 1) {
	  for (int itemp=0; itemp < veccorrij[ncycle[i]].size(); ++itemp) {
	    if (i == veccorrij[ncycle[i]][itemp]) {
	      for (int jtemp=0; jtemp < veccorrij[ncycle[i]].size(); ++jtemp) {
		if (j == veccorrij[ncycle[i]][jtemp]) {
		  double temperr_i=((TMatrixD)(VectorOfMeasScaledErrorMatrix[ncycle[i]]))(itemp,itemp);
		  double temperr_j=((TMatrixD)(VectorOfMeasScaledErrorMatrix[ncycle[i]]))(jtemp,jtemp);
		  double temperr_ij=((TMatrixD)(VectorOfMeasScaledErrorMatrix[ncycle[i]]))(itemp,jtemp);
		  tempcorrij = temperr_ij / TMath::Sqrt(temperr_i * temperr_j);
		  break;
		}
	      }
	      break;
	    }
	  }
	  //cout << " i+1 = " << i+1 << " j+1 = " << j+1 << " tempcorrij = " << tempcorrij << endl;
	}
	if (tempcorrij!=0 && i!=j) {
	  if (firstcorr) {fprintf (scaled_measfile[p], " \n"); firstcorr=false;}
	  if (icorrj[i].size() > 1) {
	    //fprintf (scaled_measfile[p], "STAT_CORR_WITH %s Gamma%s pub.%s.%s %f ! IMEAS = %d \n",
	    //	     expname[j].data(), gammaname[j].data(), author[j].data(), year[j].data(), corrmat[i][j], jjmeas);
	    fprintf (scaled_measfile[p], "STAT_CORR_WITH %s Gamma%s pub.%s.%s %f ! IMEAS = %d \n",
	    	     expname[j].data(), gammaname[j].data(), author[j].data(), year[j].data(), tempcorrij, jjmeas);
	  } else {
	    fprintf (scaled_measfile[p], "STAT_CORR_WITH %s Gamma%s pub.%s.%s %f ! IMEAS = %d \n",
		     expname[j].data(), gammaname[j].data(), author[j].data(), year[j].data(), corrmat[i][j], jjmeas);
	  }
	}
      }
      fprintf (scaled_measfile[p], " \nEND \n\n");
    }
    fclose(scaled_measfile[p]);
  }
  //
  //
  FILE *scaled_avefile[2];
  for (int p=0;p<1;++p){
    if (p==0) scaled_avefile[0]=fopen(Form("combos_average_no_babar_belle_scaled_%s%s.input",sconstrain.data(),salephhcorr.data()),"w");
    if (p==1) scaled_avefile[1]=fopen(Form("alucomb_average_no_babar_belle_scaled_%s%s.input",sconstrain.data(),salephhcorr.data()),"w");
    if (p==0) fprintf (scaled_avefile[p], "INCLUDE combos_measurements_scaled_%s%s.input \n\n",sconstrain.data(),salephhcorr.data()); 
    if (p==1) fprintf (scaled_avefile[p], "INCLUDE alucomb_measurements_scaled_%s%s.input \n\n",sconstrain.data(),salephhcorr.data()); 
    fprintf (scaled_avefile[p], "BEGIN   PDG-BABAR-BELLE all_methods \n\n");
    fprintf (scaled_avefile[p], "COMBINE * * * \n\n");
    for (ibase=0;ibase<nbase;++ibase){
      if (p==0&&ibase==(nbase-1)){/* skip */}else{
	fprintf (scaled_avefile[p], "MEASUREMENT m_Gamma%d statistical systematic   ! NQUAN = %d \n",basegamma.at(ibase),ibase+1);  
      }
    }
    if (p==0){
      fprintf (scaled_avefile[p], "\nCALL DUMP_MASTER_INC \n\n");
      fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_PRT -1.0 0. \n\n");
      fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_INV 0 0    \n\n");
    }
    //
    int lastnode=-1;
    int usum=0;//number of [unique] measurements to be expressed as linearized sum of base quantities
    for (int i=0;i<nmeas;++i){
      if (weak_re[i]==1) continue;
      vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
      inode=it-nodename.begin();
      if ((node_num_npar[inode]+node_den_npar[inode])>1) {
	if (p==1&&inode!=lastnode){//new node
	  ++usum;
	  lastnode=inode;
	  fprintf (scaled_avefile[p], "MEASUREMENT m_Gamma%s statistical systematic   ! NQUAN = %d \n",gammaname[i].data(),ibase+usum);  
	}
      }
    }
    //
    int isum=0;//number of          measurements to be expressed as linearized sum of base quantities
    lastnode=-1;
    iimeas=0;
    for (int i=0;i<nmeas;++i){
      if (weak_re[i]==1) continue;
      ++iimeas;
      vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
      inode=it-nodename.begin();
      if ((node_num_npar[inode]+node_den_npar[inode])>1) {
	//
	if (p==0&&(((inode+1)==80)||((inode+1)==82))) continue; // SPECIAL CASE [because these are derived nodes containing Gamma103 ]
	++isum; // translate C index to Fortran index
	//
	if (inode!=lastnode){//new node
	  lastnode=inode;
	}else{
	  if (p==1) continue;
	}
	//
	// PRINT NODE DEFINITION
	//
	fprintf (scaled_avefile[p], "\n* Gamma%s = ",gammaname[i].data());
	for (ipar=0; ipar < node_num_parm[inode].size(); ++ipar) {
	  if (ipar==0) { fprintf (scaled_avefile[p], "(") ; } else {fprintf (scaled_avefile[p], " + ") ;}
	  int parm=node_num_parm[inode].at(ipar);
	  vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	  int quan=ibase-baseparm.begin()+1;
	  fprintf (scaled_avefile[p], "%f*Gamma%d",node_num_coef[inode].at(ipar), basegamma[quan-1]);
	  if (ipar==node_num_parm[inode].size()-1) fprintf (scaled_avefile[p], ")");
	}
	if (node_den_parm[inode].size()==0) fprintf (scaled_avefile[p], "\n") ; 
	for (ipar=0; ipar < node_den_parm[inode].size(); ++ipar) {
	  if (ipar==0) { fprintf (scaled_avefile[p], " / (") ; } else {fprintf (scaled_avefile[p], " + ") ;}
	  int parm=node_den_parm[inode].at(ipar);
	  vector<int>::iterator ibase=find(baseparm.begin(),baseparm.end(),parm);
	  int quan=ibase-baseparm.begin()+1;
	  fprintf (scaled_avefile[p], "%f*Gamma%d",node_den_coef[inode].at(ipar), basegamma[quan-1]);
	  if (ipar==node_den_parm[inode].size()-1) fprintf (scaled_avefile[p], ")\n");
	}
	//
	double offset = -FitValue_num_re_noweak[i]; // - [ f(x0,y0) - df/dx(x=x0) x0 - df/dy(y=y0) y0 - ...]
	if (FitValue_den_re_noweak[i]>0) offset /= FitValue_den_re_noweak[i];
	//	  cout << inode << " " << node_num[inode] << " " << node_den[inode] << " " << offset << " " << measvalue[i] << endl;
	for (ipar = 0; ipar < node_parm[inode].size(); ++ipar) {
	  int parm=node_parm[inode].at(ipar);
	  int quan=node_quan[inode].at(ipar);
	  double partial=FitValue_part_re_noweak[i].at(ipar);
	  offset += partial*basevalue_fit_noweak[quan-1]; 
	}
	if (p==0) { // COMBOS
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d    %2d %d\n",isum,iimeas,node_parm[inode].size()); 
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_AD %f 1.0\n",isum,offset); 
	} else if (p==1) { // ALUCOMB
	  fprintf (scaled_avefile[p], "CONSTRAINT Gamma%s.c %f Gamma%s -1", gammaname[i].data(), offset, gammaname[i].data());
	}
	for (ipar = 0; ipar < node_parm[inode].size(); ++ipar) {
	  int parm=node_parm[inode].at(ipar);
	  int quan=node_quan[inode].at(ipar);
	  double partial=FitValue_part_re_noweak[i].at(ipar);
	  if (p==0) { // COMBOS
	    fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_%2.2d %2d %f ! Gamma%d \n",isum,ipar+1,quan,partial,basegamma.at(quan-1));
	  }else if (p==1) { // ALUCOMB
	    fprintf(scaled_avefile[p], " Gamma%d %f", basegamma[quan-1], partial);
	  }
	}
	if (p==1) fprintf(scaled_avefile[p], "\n");
      }
    }
    if (p==0) {
      iimeas=0;
      for (int i=0;i<nmeas;++i){
	if (weak_re[i]==1) continue;
	++iimeas;
	vector<string>::iterator it=find(nodename.begin(),nodename.end(),measnodename[i]);      
	inode=it-nodename.begin();
	//
	if ((inode+1)==80) { // SPECIAL CASE : NODE = 79 NAME = S035R33 GAMMA = 102 :: Gamma102 = (1.000000*Gamma103 + 1.000000*Gamma104)
	  fprintf(scaled_avefile[p], "\n*Gamma102 = 1 - Gamma3   - Gamma5   - Gamma9   - Gamma10  - Gamma14  - Gamma16\n");
	  fprintf(scaled_avefile[p], "*             - Gamma20  - Gamma23  - Gamma27  - Gamma28  - Gamma30 - Gamma35\n");
	  fprintf(scaled_avefile[p], "*             - Gamma37 - Gamma40  - Gamma42  - Gamma47  - Gamma48  - Gamma62\n");
	  fprintf(scaled_avefile[p], "*             - Gamma70 - Gamma77  - Gamma78  - Gamma85  - Gamma89  - Gamma93\n");
	  fprintf(scaled_avefile[p], "*             - Gamma94 - Gamma126 - Gamma128 - Gamma150 - Gamma152\n");
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d    %2d 29 \n",++isum,iimeas); 
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_AD -1 +1 \n",isum); // becomes a measurement of -1+Gamma102; thats why the coefficients below have - sign 
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_01  1 -1 ! Gamma3  \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_02  2 -1 ! Gamma5  \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_03  3 -1 ! Gamma9  \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_04  4 -1 ! Gamma10 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_05  5 -1 ! Gamma14 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_06  6 -1 ! Gamma16 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_07  7 -1 ! Gamma20 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_08  8 -1 ! Gamma23 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_09  9 -1 ! Gamma27 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_10 10 -1 ! Gamma28 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_11 11 -1 ! Gamma30 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_12 12 -1 ! Gamma35 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_13 13 -1 ! Gamma37 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_14 14 -1 ! Gamma40 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_15 15 -1 ! Gamma42 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_16 16 -1 ! Gamma47 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_17 17 -1 ! Gamma48 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_18 18 -1 ! Gamma62 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_19 19 -1 ! Gamma70 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_20 20 -1 ! Gamma77 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_21 21 -1 ! Gamma78 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_22 22 -1 ! Gamma85 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_23 23 -1 ! Gamma89 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_24 24 -1 ! Gamma93 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_25 25 -1 ! Gamma94 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_26 27 -1 ! Gamma126\n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_27 28 -1 ! Gamma128\n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_28 29 -1 ! Gamma150\n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_29 30 -1 ! Gamma152\n",isum);
	}
	//
	if ((inode+1)==82) { // SPECIAL CASE : NODE = 81 NAME = S035R38 GAMMA = 103 
	  fprintf(scaled_avefile[p], "\n*Gamma103 = 1 - Gamma3   - Gamma5   - Gamma9   - Gamma10  - Gamma14  - Gamma16\n");
	  fprintf(scaled_avefile[p], "*             - Gamma20  - Gamma23  - Gamma27  - Gamma28  - Gamma30 - Gamma35\n");
	  fprintf(scaled_avefile[p], "*             - Gamma37 - Gamma40  - Gamma42  - Gamma47  - Gamma48  - Gamma62\n");
	  fprintf(scaled_avefile[p], "*             - Gamma70 - Gamma77  - Gamma78  - Gamma85  - Gamma89  - Gamma93\n");
	  fprintf(scaled_avefile[p], "*             - Gamma94 - Gamma104 - Gamma126 - Gamma128 - Gamma150 - Gamma152\n");
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d    %2d 30 \n",++isum,iimeas); 
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_AD -1 +1 \n",isum); // becomes a measurement of -1+Gamma103; thats why the coefficients below have - sign 
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_01  1 -1 ! Gamma3  \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_02  2 -1 ! Gamma5  \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_03  3 -1 ! Gamma9  \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_04  4 -1 ! Gamma10 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_05  5 -1 ! Gamma14 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_06  6 -1 ! Gamma16 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_07  7 -1 ! Gamma20 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_08  8 -1 ! Gamma23 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_09  9 -1 ! Gamma27 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_10 10 -1 ! Gamma28 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_11 11 -1 ! Gamma30 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_12 12 -1 ! Gamma35 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_13 13 -1 ! Gamma37 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_14 14 -1 ! Gamma40 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_15 15 -1 ! Gamma42 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_16 16 -1 ! Gamma47 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_17 17 -1 ! Gamma48 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_18 18 -1 ! Gamma62 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_19 19 -1 ! Gamma70 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_20 20 -1 ! Gamma77 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_21 21 -1 ! Gamma78 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_22 22 -1 ! Gamma85 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_23 23 -1 ! Gamma89 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_24 24 -1 ! Gamma93 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_25 25 -1 ! Gamma94 \n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_26 26 -1 ! Gamma104\n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_27 27 -1 ! Gamma126\n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_28 28 -1 ! Gamma128\n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_29 29 -1 ! Gamma150\n",isum);
	  fprintf (scaled_avefile[p], "SPARAMETER CHI2_N_SYM_%2.2d_30 30 -1 ! Gamma152\n",isum);
	}
	//
      }
      if (p==0) fprintf (scaled_avefile[p], "\nSPARAMETER CHI2_N_SYM_NSUM  %d 0 \n",isum); 
    }
    if (p==1) {
      fprintf(scaled_avefile[p], "\n* unitarity constraint (sum of basic modes, possibly adding also dummy)\n");
      fprintf(scaled_avefile[p], "CONSTRAINT GammaAll 1\n");
      fprintf(scaled_avefile[p], "  Gamma3   1 Gamma5   1 Gamma9   1 Gamma10  1 Gamma14  1 Gamma16  1\n");
      fprintf(scaled_avefile[p], "  Gamma20  1 Gamma23  1 Gamma27  1 Gamma28  1 Gamma30  1 Gamma35  1\n");
      fprintf(scaled_avefile[p], "  Gamma37  1 Gamma40  1 Gamma42  1 Gamma47  1 Gamma48  1 Gamma62  1\n");
      fprintf(scaled_avefile[p], "  Gamma70  1 Gamma77  1 Gamma78  1 Gamma85  1 Gamma89  1 Gamma93  1\n");
      fprintf(scaled_avefile[p], "  Gamma94  1 Gamma103 1 Gamma104 1 Gamma126 1 Gamma128 1 Gamma150 1 Gamma152 1\n");
      fprintf(scaled_avefile[p], "* Gamma998 1\n");
    }
    fprintf(scaled_avefile[p], "\nCALL CHI2_N_SYM\n");
    fprintf(scaled_avefile[p], "\nEND\n");
    fclose(scaled_avefile[p]);
  }
  //
}
