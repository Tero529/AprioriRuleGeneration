#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
using namespace std;

vector<vector<vector<int> > > Frequents;
vector<vector<int> > Supports;
vector<vector<int> > Subsequent;
vector<vector<int> > Consequent;
ofstream outFile;
ifstream rf;


float MINCONF;
/////////////////////////
void getFrequents(int argc,char **argv);
int prune(vector<int>  A, vector<vector<int> > previousFrequent);
int match(vector<int>  A,vector<int>  B, int k);
float find(vector<int> itemset);
vector<vector<int> > generateCandidates(vector<vector<int> > previousFrequent,int k);
void ruleGen();
void ap_genrules(vector<int> itemset,vector<vector<int> > conseqs);
void printRules();
char  text[35][400];

////////////////////////////


int main(int argc, char **argv){
    if(argc<4){
        cout<<"Please Enter as <exe> <inputfile> <outputfile> <ConfPercentage>"<<endl;
        return 0;
    }
    outFile.open(argv[2]);

    MINCONF=(float)atoi(argv[3])/100;
    getFrequents(argc,argv);
    ruleGen();
    printRules();

}

void printRules(){
  rf.open("rules.th");

  char line[400];
  int i=1;
  while(rf>>text[i++])
  ;

  for(int i=0;i<Subsequent.size();i++){
    outFile<<"Rule "<<i+1<<": ";
    for(int j=0;j<Subsequent[i].size();j++){
      outFile<<text[Subsequent[i][j]]<<" ";
    }
    outFile<<" --->  ";
    for(int j=0;j<Consequent[i].size();j++){
      outFile<<text[Consequent[i][j]]<<" ";
    }
    outFile<<endl;
  }

}
vector<vector<int> > generateCandidates(vector<vector<int> > previousFrequent,int k){
    vector<vector<int> > candidates;
    vector<int> f;
    //Make Comparisions for k-1 identical elements

    for(int i=0;i<previousFrequent.size()-1;i++){
      for(int j=i+1;j<previousFrequent.size();j++){
        //Add element from second itemset if matched
        if(match(previousFrequent[i],previousFrequent[j],previousFrequent[i].size()-1)){
            f.assign(previousFrequent[i].begin(),previousFrequent[i].end());
            f.push_back(previousFrequent[j][(previousFrequent[j].size())-1]);
            sort(f.begin(),f.end());
            //Check if all k-1 sized subsets are frequent
            if(prune(f,previousFrequent)){
              candidates.push_back(f); //Add to List of candidate itemsets
            }
        }
      }
    }
  //  cout<<"Candidates are: "<<candidates.size()<<" of size "<<k<<endl;
    return candidates;
}

float find(vector<int> itemset){
  int index=itemset.size();
  for(int i=0;i<Frequents[index].size();i++){
    if(match(itemset,Frequents[index][i],index))
      return Supports[index][i];
  }
  return -1;
}

void ap_genrules(vector<int> itemset,vector<vector<int> > conseqs){

  int k=itemset.size();
  int m= conseqs.front().size();
  vector<vector<int> > candidates =vector<vector<int> >();
  vector<vector<int> >toPass=vector<vector<int> >();
int flag=0;

  if(k > m+1){
  //  cout<<k<<" "<<m<<endl;
    float numerator=find(itemset);
    candidates=generateCandidates(conseqs,m);
    for(int i=0;i<candidates.size();i++){//For each Candidate
      vector<int> copy;
      copy.assign(itemset.begin(),itemset.end());
      //Set Difference
      for(int j=0;j<candidates[i].size();j++){
        for(int k=0;k<copy.size();k++){
          if(candidates[i][j]==copy[k]){
            copy.erase(copy.begin()+k);
            break;
          }
        }
      }

      float denominator=find(copy);
      float confidence=numerator/denominator;
      if(confidence>MINCONF){
        Subsequent.push_back(copy);
        Consequent.push_back(candidates[i]);
        toPass.push_back(candidates[i]);
        flag=1;
      }

    }
    if(flag)
    ap_genrules(itemset,toPass);
  }
  else{
  //  cout<<"Quit"<<endl;
    return;
  }
}


void ruleGen(){
  for(int i=2;i<Frequents.size();i++){
    if(Frequents[i].size()==0)
      continue;
    for(int j=0;j<Frequents[i].size();j++){
      vector<vector<int> > conseqs;
      for(int k=0;k<Frequents[i][j].size();k++){
        vector<int> cons;
        cons.push_back(Frequents[i][j][k]);
        conseqs.push_back(cons);
      }
      ap_genrules(Frequents[i][j],conseqs);
    }
  }
}



void getFrequents(int argc,char **argv){
  ifstream inFile;
  inFile.open(argv[1]);
  int size,sup,item;
  vector<int> itemset ;
  vector<int> sups;

  Frequents.resize(15);
  Supports.resize(15);


  while(inFile){
    inFile>>size;
    inFile>>sup;
    for(int i=0;i<size;i++){
      inFile>>item;
      itemset.push_back(item);
    }
    Frequents[size].push_back(itemset);
    Supports[size].push_back(sup);
    itemset.clear();
  }
}


int prune(vector<int>  A, vector<vector<int> > previousFrequent){

  //Check k itemsets of size k-1;>
  vector<int>  subset;
  int flag=0;
/*  cout<<"Itemset to Prune"<<endl;
  for(int m=0;m<A.size();m++)
    cout<<A[m]<<" ";
    cout<<endl<<"---------------"<<endl;
*/
  for(int i=0;i<A.size();i++){
    subset=A;
    flag=0;
    subset.erase(subset.begin()+i);
    for(int j=0;j<previousFrequent.size();j++){
      if(match(subset,previousFrequent[j],subset.size())==1){
        flag=1;
        break;
      }
    }
    if(flag==0)
      return 0;
  }
  for(int i=0;i<A.size()-1;i++){
    if(A[i]%2 == 1){
      if(A[i+1]==A[i]+1)
        return 0;
    }
  }

  return 1;
}

int match(vector<int> A,vector<int>  B, int k){
  for(int i=0;i<k;i++){
    if(A[i]!=B[i])
      return 0;
  }
  return 1;
}
