/*
 * C++ Segment Tree implementation
 * Operations: 
 *  - Get sum: compute the sum of the elements in range [l, r).                 O(log N)
 *  - Add: add an integer x to all the elements in range [l, r).                O(log N)
 *  - Set range: set all the elements in range [l, r) equal to x.               O(log N)
 *  - Get min: compute the minimum of the elements in range [l, r).             O(log N)
 *  - Lower bound: compute the index of the leftmost element in range [l, r)    O(log N)
 *    which does not exceed x.
*/


#include <vector>
#include <limits>
#include <iostream>
using namespace std;

template<typename K>
class Segtree{
private:

    struct Node{
        static const K MAX=numeric_limits<K>::max();
        static const K SET=numeric_limits<K>::min();

        K sum;
        K lazyadd;
        K lazyset;
        K minn;
        Node(): sum(0),minn(MAX),lazyadd(0),lazyset(SET) {};
        void join(Node &a,Node &b){
            sum=a.sum+b.sum;
            minn=min(a.minn,b.minn);
        }
    };

    vector<Node>segtree;
    int Size;

public:

    Segtree(vector<K>&data){
        Size=getSize(data.size());
        segtree.assign(2*Size,Node());
        for(int i=0;i<data.size();i++){
            segtree[i+Size].sum=data[i];
            segtree[i+Size].minn=data[i];
        }
        for(int i=Size-1;i>0;i--)
            segtree[i].join(segtree[i*2],segtree[i*2+1]);
    }

    K getSum(int l,int r){
        return getSum(1,0,Size,l,r);
    }

    K getMin(int l,int r){
        return getMin(1,0,Size,l,r);
    }

    int lowerBound(int l,int r,K x){
        return lowerBound(1,0,Size,l,r,x);
    }

    void add(int l,int r,K x){
        add(1,0,Size,l,r,x);
    }

    void setRange(int l,int r,K x){
        setRange(1,0,Size,l,r,x);
    }

private:

    int getSize(int n){
        int newSize=1;
        while(newSize<n)
            newSize*=2;
        return newSize;
    }

    void propagate(int node,int l,int r){
        if(segtree[node].lazyadd==0 && segtree[node].lazyset==Node::SET)
            return;

        if(segtree[node].lazyadd!=0){
            segtree[node].sum+=(r-l)*segtree[node].lazyadd;
            segtree[node].minn+=segtree[node].lazyadd;
            if(node*2+1<segtree.size()){
                if(segtree[node*2].lazyset!=Node::SET){
                    segtree[node*2].lazyset+=segtree[node].lazyadd;
                    segtree[node*2].lazyadd=0;
                }
                else
                    segtree[node*2].lazyadd+=segtree[node].lazyadd;

                if(segtree[node*2+1].lazyset!=Node::SET){
                    segtree[node*2+1].lazyset+=segtree[node].lazyadd;
                    segtree[node*2+1].lazyadd=0;
                }
                else
                    segtree[node*2+1].lazyadd+=segtree[node].lazyadd;
            }
            segtree[node].lazyadd=0;
            return;
        }
        if(segtree[node].lazyset!=Node::SET){
            segtree[node].sum=(r-l)*segtree[node].lazyset;
            segtree[node].minn=segtree[node].lazyset;
            if(node*2+1<segtree.size()){
                segtree[node*2].lazyset=segtree[node].lazyset;
                segtree[node*2].lazyadd=0;
                segtree[node*2+1].lazyset=segtree[node].lazyset;
                segtree[node*2+1].lazyadd=0;
            }
            segtree[node].lazyset=Node::SET;
        }
    }

    K getSum(int node,int l,int r,int lq,int rq){
        propagate(node,l,r);

        if(l>=rq || r<=lq)
            return 0;
        if(l>=lq && r<=rq)
            return segtree[node].sum;

        return getSum(node*2,l,(l+r)/2,lq,rq)+getSum(node*2+1,(l+r)/2,r,lq,rq);
    }

    K getMin(int node,int l,int r,int lq,int rq){
        propagate(node,l,r);

        if(l>=rq || r<=lq)
            return Node::MAX;
        
        if(l>=lq && r<=rq)
            return segtree[node].minn;

        return min(getMin(node*2,l,(l+r)/2,lq,rq),getMin(node*2+1,(l+r)/2,r,lq,rq));
    }

    int lowerBound(int node,int l,int r,int lq,int rq,K x){
        propagate(node,l,r);

        if(l>=rq || r<=lq)
            return -1;

        if(segtree[node].minn<=x){
            if(!(node*2+1<segtree.size()))
                return l;

            int temp=lowerBound(node*2,l,(l+r)/2,lq,rq,x);
            if(temp!=-1)
                return temp;

            return lowerBound(node*2+1,(l+r)/2,r,lq,rq,x);
        }
        return -1;
    }

    void add(int node,int l,int r,int lq,int rq,K x){
        propagate(node,l,r);

        if(l>=rq || r<=lq)
            return;

        if(l>=lq && r<=rq){
            segtree[node].sum+=(r-l)*x;
            segtree[node].minn+=x;
            if(node*2+1<segtree.size()){
                if(segtree[node*2].lazyset!=Node::SET)
                    segtree[node*2].lazyset+=x;
                else
                    segtree[node*2].lazyadd+=x;

                if(segtree[node*2+1].lazyset!=Node::SET)
                    segtree[node*2+1].lazyset+=x;
                else
                    segtree[node*2+1].lazyadd+=x;
            }
            return;
        }

        add(node*2,l,(l+r)/2,lq,rq,x);
        add(node*2+1,(l+r)/2,r,lq,rq,x);
        segtree[node].join(segtree[node*2],segtree[node*2+1]);
    }

    void setRange(int node,int l,int r,int lq,int rq,K x){
        propagate(node,l,r);

        if(l>=rq || r<=lq)
            return;

        if(l>=lq && r<=rq){
            segtree[node].minn=x;
            segtree[node].sum=(r-l)*x;

            if(node*2+1<=segtree.size()){
                segtree[node*2].lazyset=x;
                segtree[node*2].lazyadd=0;
                segtree[node*2+1].lazyset=x;
                segtree[node*2+1].lazyadd=0;
            }
            return;
        }
        setRange(node*2,l,(l+r)/2,lq,rq,x);
        setRange(node*2+1,(l+r)/2,r,lq,rq,x);
        segtree[node].join(segtree[node*2],segtree[node*2+1]);
    }
};