/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "TMVAWrapper.h" 
#include "RooAbsReal.h" 

#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 


#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif

ClassImp(TMVAWrapper) 


void TMVAWrapper::findVariables(TXMLNode *node)
{
  // is this one a Variable?
  if (!strcmp(node->GetNodeName(),"Variable") && node->HasAttributes())
    {

      // if so, get the name
      TList *atts = node->GetAttributes();
      TXMLAttr *ob = (TXMLAttr*)atts->FindObject("Label");
      //      std::cout << "   @@@@ found feature " << ob << " " << ob->GetValue() <<  std::endl;
      if (names.size()>_max_vars)
	{
	  std::cout << "--> Error max vars exceeded: " << _max_vars <<std::endl;
	}
      else
	names.push_back(ob->GetValue());
    }

  // recursively explore siblings and children
  if (node->HasChildren()) findVariables(node->GetChildren());
  if (node->HasNextNode()) 
    findVariables(node->GetNextNode());
}

void TMVAWrapper::getListOfVars(char weights[1000])
{
  num_features=0;
  names.clear();
  
  // set up the parser
  TDOMParser *parser = new TDOMParser();

  // validation looks for a DTD file, we don't have one
  parser->SetValidate(false);

  // parse the file
  parser->ParseFile(weights);

  // get the root of the XML tree
  TXMLDocument *doc = parser->GetXMLDocument();
  TXMLNode *node = doc->GetRootNode();
  
  // explore it recursively, looking for variables
  findVariables(node);

  num_features=names.size();

}


TMVAWrapper::TMVAWrapper(const char *name, const char *title,RooArgList &ral,char weights_in[1000]):
  RooAbsReal(name,title),
  _actualVars("actualVars","useless title",this)
 { 

   _actualVars.add(ral);
   
   TMVA::Tools::Instance();
   reader = new TMVA::Reader( "!Color:!Silent" );    

   // keep a copy of the filename
   strcpy(weights,weights_in);

   // read the XML to find the list of NN input variables
   getListOfVars(weights);

   //   local_features = new Float_t[num_features];
   // for each variable, tell the TMVA reader to look for it
   for (int i=0;i<num_features;i++)
     reader->AddVariable(names[i],&local_features[i]);
     
   // --- Book the MVA methods
   TString dir    = "weights/";
   TString prefix = "TMVARegression";

   // Book method(s)
   TString methodName = "MLP method";
   reader->BookMVA( methodName, weights);
 } 

 TMVAWrapper::TMVAWrapper(const TMVAWrapper& other, const char* name) :  
   RooAbsReal(other,name),
   _actualVars("actualVars",this,other._actualVars)
 { 
   
    reader=NULL;

   TMVA::Tools::Instance();
   reader = new TMVA::Reader( "!Color:!Silent" );    
   
   // get name of weights file
   strcpy(weights,other.weights);

   // get list of features
   num_features = other.num_features;
   names.clear();
   for (int i=0;i<num_features;i++)
     names.push_back(other.names[i]);

   // register them
   for (int i=0;i<num_features;i++)
     reader->AddVariable(names[i],&local_features[i]);

   // --- Book the MVA methods

   TString dir    = "weights/";
   TString prefix = "TMVARegression";

   // Book method(s)
   TString methodName = "MLP method";
   reader->BookMVA( methodName, other.weights ); 

 } 


 Double_t TMVAWrapper::evaluate() const 
 { 

   // put the data into local storage from Roo...
   for (int i=0;i<num_features;i++)
     local_features[i] = ((RooAbsReal*)_actualVars.at(i))->getVal();

   // call the network -- we have already told it where to find the data
   Float_t val = (reader->EvaluateRegression("MLP method"))[0];

   // return it
   return val ; 
 } 

