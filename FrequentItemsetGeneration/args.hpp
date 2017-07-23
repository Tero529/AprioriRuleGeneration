#ifndef ARGS_H
#define ARGS_H

using namespace std;
#include<vector>
#include<stack>
typedef struct Nodet node;
int minCount;

struct Nodet{
  bool leaf;
  int children;
  int size;
  node *child[3];
  vector<vector<int> > bucket;
  vector<int> supports;
};

bool compare(vector<int> A,vector<int> B){
  for(int i=0;i<A.size();i++){
    if(A[i]!=B[i])
      return false;
  }
  return true;
}

void traverse(node *head,vector<vector<int> > *freqs,vector<int> *fsups){

  if(head==NULL)
    return;

  stack<node *> st;
  st.push(head);

  while(st.empty() ==false){
    node *temp=st.top();
    for(int i=0;i<temp->bucket.size();i++){

      if(temp->supports[i]>minCount){
        int x=temp->supports[i];
        cout<<temp->bucket[i].size()<<" ";
        cout<<x<<" ";
        for(int m=0;m<temp->bucket[i].size();m++)
          cout<<temp->bucket[i][m]<<" ";
        cout<<endl;
        freqs->push_back(temp->bucket[i]);
        fsups->push_back(x);
      }
      //if(temp->supports[i]>minCount)
      //cout<<temp->bucket[i].size()<<endl<<temp->supports[i]<<"\n-----\n";
    }
    st.pop();
    for(int i=0;i<3;i++){
      if(temp->child[i] != 0x0)
        st.push(temp->child[i]);
    }
  }
}

void hasher(node **head,vector<int> candidate,bool insert){
  int k=0;
//  cout<<"Trying to hash : ";

  node *root = (*head);
  while(1){

    if(k==candidate.size()){
        if(insert){
          (root->bucket).push_back(candidate);
          (root->supports).push_back(0);
          root->size +=1;
        }
        else{
          for(int i=0;i<(root->bucket).size();i++){
            if(compare(root->bucket[i],candidate))
                root->supports[i]=root->supports[i]+1;
              //  cout<<"But oer here "<<root->supports[i]<<endl;

                //if(root->supports[i]>minCount){
                /*    freq->push_back(candidate);
                    (root->bucket).erase((root->bucket).begin()+i);
                    (root->supports).erase((root->supports).begin()+i);
                    i--;
                }*/
          }
        }
      break;
    }

    root->leaf= false;
    int hash=candidate[k] % 3;
    if(root->child[hash] == NULL)
      root->child[hash]=new node();
    root=root->child[hash];
    k++;
  }
}

#endif
