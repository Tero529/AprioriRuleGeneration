#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<cstdio>

#include "args.hpp"

using namespace std;
typedef vector<int> itemset;
typedef vector<int> transaction;

#define MINCONF 90
vector<transaction> Data;
vector<vector<itemset> > Frequents;
vector<vector<int> > freqSupport;
///////////////////////
transaction parseLine(char *line);
int prune(itemset A, vector<itemset> previousFrequent);
int match(itemset A,itemset B, int k);
vector<vector <int> > numerate(vector<int> trans,int k);
vector<transaction > makeInstances(char **argv);
vector<itemset> generateCandidates(vector<itemset> previousFrequent,int k);
vector<vector<itemset> > generateFrequentItemsets(vector<transaction> Data);
///////////////////////


//-------------------------------
int itemSupports[35];
//-------------------------------

int main(int argc,char **argv){

    if(argc<3){
        cout<<"Please Enter as <exe> <datafile> <supPercentage>"<<endl;
        return 0;
    }
    float MINSUP=atoi(argv[2]);
    Data=makeInstances(argv);
    minCount=((float)MINSUP/100)*Data.size();
    //cout<<minCount<<endl;

    Frequents=generateFrequentItemsets(Data);

    for(int i=1;i<35;i++){
      if(itemSupports[i]>minCount){
        cout<<1<<" "<<itemSupports[i]<<" "<<i<<endl;
      }
    }
    return 0;
}


vector<vector<itemset> > generateFrequentItemsets(vector<transaction> Data){

    vector<vector<itemset> > frequents;
    int k=1;//Generates k+1 size frequent itemsets
    itemset temp;
    vector<itemset> candidates;

    //Generate frequent items
    for(int i=1;i<=34;i++){
        temp.clear();
        if(itemSupports[i]>minCount){
            temp.push_back(i);
            candidates.push_back(temp);
        }
    }
  //  cout<<"Frequent items "<<candidates.size()<<endl;
    frequents.push_back(candidates);
    k++;
    node *root;
    //while(frequents.back().size()!=0)
    while(1){
        vector<vector<int> > toPush;
        vector<int> toPushVals;
        candidates=generateCandidates(frequents.back(),k);
        //cout<<candidates.size()<<" Candidates of itemset size "<<k<<endl;
        if(candidates.size()==0)
          break;
      //  cout<<"Generated Candidates\n";
        root=new node();


        for(int i=0;i<candidates.size();i++){
          hasher(&root,candidates[i],true);
        }
      //cout<<"Hashed Candidates\n";
        for(int i=0;i<Data.size();i++){
            vector<vector<int> > fromTrans;
            fromTrans = numerate(Data[i],k);
            for(int j=0;j<fromTrans.size();j++)
              hasher(&root,fromTrans[j],false);
        }

        traverse(root,&toPush,&toPushVals);

        if(toPush.size()==0)
          break;
    //    cout<<"Number of Frequent Itemsets : "<<toPush.size()<<endl<<"----\n";
        frequents.push_back(toPush);
        freqSupport.push_back(toPushVals);
        k++;

    }

    return frequents;
}

void numHelp(vector<vector<int> > *Result,vector<int> trans,int k, vector<int> current,int index){
  if(current.size()==k){
    Result->push_back(current);
    return;
  }
  else if(index>=trans.size()){

    return;
  }
  else{
      numHelp(&(*Result),trans,k,current,index+1);
      current.push_back(trans[index]);
      numHelp(&(*Result),trans,k,current,index+1);
  }
}


vector<vector<int> > numerate(vector<int> trans,int k){
  vector<vector<int> > ret;
  vector<int> temp= vector<int> ();
  numHelp(&ret,trans,k,temp,0);
  return ret;
}


vector<itemset> generateCandidates(vector<itemset> previousFrequent,int k){
    vector<itemset> candidates;
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

int prune(itemset A, vector<itemset> previousFrequent){

  //Check k itemsets of size k-1;
  itemset subset;
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

int match(itemset A,itemset B, int k){
  for(int i=0;i<k;i++){
    if(A[i]!=B[i])
      return 0;
  }
  return 1;
}

vector<transaction> makeInstances(char **argv){
    ifstream inFile;
    char line[100];
    vector<transaction > Instances;

    inFile.open(argv[1]);
    while(inFile){
        inFile>>line;
        if(strcmp("@data",line)==0)
            break;
    }
    while(inFile){
        inFile>>line;
        if(strcmp(line,"%")==0)
            break;
        Instances.push_back(parseLine(line));
    }
    return Instances;
}

transaction parseLine(char *line){

    transaction instance;
    line++;
    for(int i=0;i<17;i++){
        if(line[0]=='y' || line[0]=='r'){
            instance.push_back(i*2+1);
            itemSupports[i*2+1]++;
            line+=3;
        }
        else if(line[0]=='n' || line[0]=='d'){
            instance.push_back(i*2+2);
            itemSupports[i*2+2]++;
            line+=3;
        }
        else{
            line+=2;
        }
        if(line[0]!='?')
            line++;
    }

    return instance;

}
