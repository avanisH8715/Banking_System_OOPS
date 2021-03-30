#include<bits/stdc++.h>
#include<iostream>
#include<string>
#include<vector>
using namespace std;


class Account  //abstract class
{
public:
    int Acc_No;
    string type;
    int Balance;

    static int next_number;
    Account(int b)
    {
        Acc_No=next_number;
        Balance=b;
        next_number++;
    }

    virtual bool debitAmount(int money)=0;

    void creditAmount(int money)
    {
      Balance+=money;
    }

    int getBalance()
    {
        return Balance;
    }
};

int Account::next_number=0;

class Saving_Account:public Account
{
public:
    int Min_Balance;
    int sint_rate;

    Saving_Account(int b):Account(b)
    {
        Min_Balance=500;
        sint_rate=10;
        type="SA";
    }

    virtual bool debitAmount(int money)
    {
        if((Balance-money)<500)
         {
             return 0;
         }
        else
        {
            Balance-=money;
            return 1;
        }
    }
};

class Current_Account:public Account{
public:
    int cint_rate;

    Current_Account(int b):Account(b)
    {
        cint_rate=5;
        type="CA";
    }

    virtual bool debitAmount(int money)
    {
        if((Balance-money)<0)
            return 0;
        else
        {
            Balance-=money;
            return 1;
        }
    }
};
map<int,Saving_Account*> c_id2saccptr;
map<int,Current_Account*> c_id2caccptr;

class Branch
{
public:
    int Branch_code;
    string city;
    vector<Saving_Account> sa;
    vector<Current_Account> ca;

    Branch(){}

    Branch(int bcode,string c)
    {
        Branch_code=bcode;
        city=c;
    }

    void addAccount(int custid)
    {
        string type;
         int balance;
        cout<<"Enter type of account (SA/CA): ";
        cin>>type;
        if(type=="SA")
        {
            cout<<"Please enter initial balance > 500 for savings account"<<endl;
            cout<<"Enter initial balance: ";
            cin>>balance;
            Saving_Account s(balance);
            sa.push_back(s);
            cout<<"Account created"<<endl;
            Saving_Account *sav=new Saving_Account(balance);
            c_id2saccptr.insert(pair<int,Saving_Account*>(custid,sav));
        }
        else
        {
            cout<<"Enter initial balance: ";
            cin>>balance;
            Current_Account c(balance);
            ca.push_back(c);
            cout<<"Account created"<<endl;
            Current_Account *curr=new Current_Account(balance);
            c_id2caccptr.insert(pair<int,Current_Account*>(custid,curr));
        }
    }

    void removeAccount(int cid);

} ;


class Bank
{
public:
    string name,code;
    vector<Branch> branches;
    Branch main_b;

    Bank()
    {
    }

    Bank(string n,string c,Branch &b)
    {
            name=n;
            code=c;
            main_b=b;
    }

    void addBranch(Branch &b1)
    {
        int flag=1;
        for(auto b:branches)
        {
            if(b.Branch_code==b1.Branch_code)
                flag=0;
        }
        if(flag)
            branches.push_back(b1);
    }

    void RemoveBranch(Branch &b1)
    {
        int temp;
        for(int i=0;i<branches.size();i++)
        {
            if(branches[i].Branch_code==b1.Branch_code)
            {
                temp=i;
                break;
            }
        }
        branches.erase(branches.begin()+temp);
    }

    void getBranch()
    {
        cout<<"Main branch: "<<main_b.city<<" "<<main_b.Branch_code<<endl;
    }

    void getAllBranches()
    {
        cout<<"Displaying all branches of bank : "<<this->name<<endl;
        cout<<"Branch code"<<"\t"<<"Branch name"<<"\t"<<endl;
        for(auto b:branches)
        {
            cout<<b.Branch_code<<"\t"<<b.city<<"\t"<<endl;

        }
    }
};

map<int,Branch*> b_code2brptr;
map<int,Branch*> c_id2brptr;
map<int,string> c_id2c_name;

void Branch::removeAccount(int cid)
{
            Branch *br1=c_id2brptr[cid];
            if(!(c_id2saccptr.find(cid)==c_id2saccptr.end()))
            {
                int i;
                Saving_Account* sptr=c_id2saccptr[cid];
                for(i=0;i<(br1->sa).size();i++)
                {
                    if(((br1->sa)[i]).Acc_No==(sptr->Acc_No))
                        break;
                }
                (br1->sa).erase(i+(br1->sa).begin());
            }
            else if(!(c_id2caccptr.find(cid)==c_id2caccptr.end()))
            {
                int i;
                Current_Account* cptr=c_id2caccptr[cid];
                for(i=0;i<(br1->ca).size();i++)
                {
                    if(((br1->ca)[i]).Acc_No==(cptr->Acc_No))
                        break;
                }
                (br1->ca).erase(i+(br1->ca).begin());
            }
            else
            {
            }
}

class Customer
{
public:
    int Cust_ID,phonenum;
    static int next_id;
    string name,address;

    Customer(string n,int p,string a)
    {
        name=n;
        phonenum=p;
        address=a;
        Cust_ID=next_id;
        next_id++;
    }

    void addaccount()
    {
            int b_id;
            cout<<"Enter branch code of that branch in which you want to add account: ";
            cin>>b_id;
            c_id2c_name.insert(pair<int,string>(this->Cust_ID,name));
            Branch *ptr=b_code2brptr[b_id];
            c_id2brptr.insert(pair<int,Branch*>(this->Cust_ID,ptr));
            ptr->addAccount(this->Cust_ID);
    }

    void deposit()
    {
            int d,cid;

            cout<<"Displaying customer IDs: "<<endl;
            map<int,Saving_Account*>::iterator itr1;
            map<int,Current_Account*>::iterator itr2;
            for(itr1=c_id2saccptr.begin();itr1!=c_id2saccptr.end();itr1++)
            {
                cout<<itr1->first<<" ";
            }
            for(itr2=c_id2caccptr.begin();itr2!=c_id2caccptr.end();itr2++)
            {
                cout<<itr2->first<<" ";
            }
            cout<<endl;

            cout<<"Enter customer ID to whose account you want to deposit: ";
            cin>>cid;
            cout<<"Enter deposit money: ";
            cin>>d;

            Branch *br=c_id2brptr[cid];
            if(!(c_id2saccptr.find(cid)==c_id2saccptr.end()))
            {
                Saving_Account* sav=c_id2saccptr[cid];
                sav->creditAmount(d);
                for(auto v:br->sa)
                {
                    v.creditAmount(d);
                }
            }
            else
            {
                Current_Account* curr=c_id2caccptr[cid];
                curr->creditAmount(d);
                for(auto v:br->ca)
                {
                    v.creditAmount(d);
                }
            }
            cout<<"Amount deposit successful"<<endl;
    }

    void withdraw()
    {
            int cid,w;
            cout<<"Displaying customer IDs: "<<endl;
            map<int,Saving_Account*>::iterator itr1;
            map<int,Current_Account*>::iterator itr2;
            bool flag;
            for(itr1=c_id2saccptr.begin();itr1!=c_id2saccptr.end();itr1++)
            {
                cout<<itr1->first<<" ";
            }
            for(itr2=c_id2caccptr.begin();itr2!=c_id2caccptr.end();itr2++)
            {
                cout<<itr2->first<<" ";
            }
            cout<<endl;

            cout<<"Enter customer ID from whose account you want to withdraw: ";
            cin>>cid;
            cout<<"Enter withdrawal money: ";
            cin>>w;

            Branch *b=c_id2brptr[cid];
            if(!(c_id2saccptr.find(cid)==c_id2saccptr.end()))
            {
                for(auto v:b->sa)
                {
                    flag=v.debitAmount(w);
                }
                Saving_Account* sav=c_id2saccptr[cid];
                flag=sav->debitAmount(w);

                if(flag)
                    cout<<"Amount withdraw successful"<<endl;
                else
                    cout<<"Insufficient funds..balance will be less than 500 in savings account"<<endl;
            }
            else
            {
               for(auto v:b->ca)
                {
                    flag=v.debitAmount(w);
                }
                 Current_Account* curr=c_id2caccptr[cid];
                 flag=curr->debitAmount(w);
                 if(flag)
                    cout<<"Amount withdraw successful"<<endl;
                else
                    cout<<"Insufficient funds.."<<endl;
            }

        }

        void remove_account(int cid)
        {

            c_id2c_name.erase(cid);
            if(!(c_id2saccptr.find(cid)==c_id2saccptr.end()))
            {
                c_id2saccptr.erase(cid);
            }
            else if(!(c_id2caccptr.find(cid)==c_id2caccptr.end()))
            {
                c_id2caccptr.erase(cid);
            }
            else
            {
                cout<<"No such customer id exist"<<endl;
            }
        }


};

int Customer::next_id=1001;


int main()
{
    Branch *b1=new Branch(1112,"jamshedpur");
    Bank *bank=new Bank("SBI","SBI123",*b1);
    Branch *b2=new Branch(1114,"jaipur");
    Branch *b3=new Branch(1119,"Gwalior");
    Branch *b4=new Branch(1116,"Banaras");
    Branch *b5=new Branch(1125,"Raipur");

    bank->addBranch(*b1);
    bank->addBranch(*b2);
    bank->addBranch(*b3);
    bank->addBranch(*b4);
    bank->addBranch(*b5);
    vector<Branch*> branches{b1,b2,b3,b4,b5};
    b_code2brptr.insert(pair<int,Branch*>(1112,b1));
    b_code2brptr.insert(pair<int,Branch*>(1114,b2));
    b_code2brptr.insert(pair<int,Branch*>(1119,b3));
    b_code2brptr.insert(pair<int,Branch*>(1116,b4));
    b_code2brptr.insert(pair<int,Branch*>(1125,b5));
    int ch,b_id;

    cout<<"WELCOME"<<endl;
    while(1)
    {
        cout<<"************************************"<<endl;
        cout<<"1.ADD ACCOUNT"<<endl;
        cout<<"2.DEPOSIT MONEY"<<endl;
        cout<<"3.WITHDRAW MONEY"<<endl;
        cout<<"4.REMOVE ACCOUNT"<<endl;
        cout<<"5.GET DETAILS OF A CUSTOMER"<<endl;
        cout<<"6.Exit"<<endl;
        cout<<"************************************"<<endl<<endl;

        cout<<"Enter your choice: ";
        cin>>ch;
        Customer* cust;
        if(ch==1)
        {
            int phonenum;
            string name,address;
            cout<<"Enter customer details:- "<<endl;
            cout<<"Enter name: ";
            cin>>name;
            cout<<"Phone number: ";
            cin>>phonenum;
            cout<<"Address: ";
            cin>>address;
            Customer *cobj=new Customer(name,phonenum,address);
            cust=cobj;
            bank->getAllBranches();
            cobj->addaccount();

        }
        else if(ch==2)
         {
             cust->deposit();
         }

        else if(ch==3)
        {
            cust->withdraw();
        }

        else if(ch==4)
        {
            int cid;
            cout<<"Displaying customer IDs: "<<endl;
            map<int,Saving_Account*>::iterator itr1;
            map<int,Current_Account*>::iterator itr2;
            for(itr1=c_id2saccptr.begin();itr1!=c_id2saccptr.end();itr1++)
            {
                cout<<itr1->first<<" ";
            }
            for(itr2=c_id2caccptr.begin();itr2!=c_id2caccptr.end();itr2++)
            {
                cout<<itr2->first<<" ";
            }
            cout<<endl;

            cout<<"Enter customer id whose account to be deleted: ";
            cin>>cid;
            cust->remove_account(cid);
            Branch *br1=c_id2brptr[cid];
            br1->removeAccount(cid);
            cout<<"Account deleted successfully"<<endl;
        }

    else if(ch==5)
    {
        cout<<"Available IDs of Customers: "<<endl;
        int id;
        Customer* cust;
        map<int,Saving_Account*>::iterator itr1;
        map<int,Current_Account*>::iterator itr2;
        for(itr1=c_id2saccptr.begin();itr1!=c_id2saccptr.end();itr1++)
            {
                cout<<itr1->first<<" ";
            }
        for(itr2=c_id2caccptr.begin();itr2!=c_id2caccptr.end();itr2++)
            {
                cout<<itr2->first<<" ";
            }
        cout<<endl;
        cout<<"Enter customer id whose details you want: ";
        cin>>id;
        if(!(c_id2saccptr.find(id)==c_id2saccptr.end()))
        {
            Saving_Account *s=c_id2saccptr[id];
            cout<<"Account number: "<<s->Acc_No<<endl;
            cout<<"Account type: "<<s->type<<endl;
            cout<<"Balance: "<<s->Balance<<endl;
        }
        else
        {
            Current_Account *c=c_id2caccptr[id];
            cout<<"Account number: "<<c->Acc_No<<endl;
            cout<<"Account type: "<<c->type<<endl;
            cout<<"Balance: "<<c->Balance<<endl;
        }
        cout<<"Customer name: "<<c_id2c_name[id]<<endl;
    }
    else
    {
        delete b1,b2,b3,b4,b5,bank,cust;
        cout<<"Thank you"<<endl;
        exit(1);
    }

    }
    return 0;
}
