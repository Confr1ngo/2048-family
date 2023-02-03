//  _  _       _                   _                  
// | \| |___  | |__ _  _ __ _ ___ | |_  ___ _ _ ___   
// | .` / _ \ | '_ \ || / _` (_-< | ' \/ -_) '_/ -_)_ 
// |_|\_\___/ |_.__/\_,_\__, /__/ |_||_\___|_| \___(_)
//                      |___/                         

//  ___  _           ___         ___               _  _    _ 
// |   \(_)_ _____  | _ )_  _   / __|___ _ _  __ _| || |_ | |
// | |) | \ V / -_) | _ \ || | | (_ / _ \ ' \/ _` | __ | || |
// |___/|_|\_/\___| |___/\_, |  \___\___/_||_\__, |_||_|\__/ 
//                       |__/                |___/           
#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
using namespace std;
int pr[10010]={2},prcnt=1,show0=0;
int num[10][10],score,n;
int ad[10010],adcnt,eli[10010],elicnt;
set<int>s;
void setFontColor(int ForgC,int BackC){
	WORD wColor=((BackC&0x0F)<<4)+(ForgC&0x0F);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),wColor);
}
struct Position{
	int x,y;
	Position(){x=y=0;}
	Position(int xx,int yy){x=xx;y=yy;}
	Position& operator=(const Position& p){
		x=p.x;y=p.y;return *this;
	}
};
void GameOver(){
	cout<<"Game Over.";
	while (true);
}
void ClearScreen(){
	system("cls");
}
int getLen(int nn){
	if (nn==0) return 1;
	int retval=0;
	while (nn>0){
		nn/=10;
		retval++;
	}
	return retval;
}
int getMaxLen(){
	int retval=INT_MIN;
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			retval=max(retval,getLen(num[i][j]));
		}
	}
	return retval;
}
void PrintStatus(){
	cout<<"Seeds:";
	for (int i=0;i<prcnt;i++){
		cout<<" "<<pr[i];
	}
	cout<<"\nScore: "<<score<<"\n";
	int maxlen=getMaxLen();
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			if (num[i][j]==0){
				if (show0==2){
					setFontColor(8,0);
					cout<<setw(maxlen+1)<<num[i][j];
					setFontColor(7,0);
				}
				else if (show0==1) cout<<setw(maxlen+1)<<num[i][j];
			}
			else{
				if (show0==2) setFontColor(15,0);
				cout<<setw(maxlen+1)<<num[i][j];
				if (show0==2) setFontColor(7,0);
			}
		}
		cout<<"\n";
	}
	int sumx=0;
	cout<<"Eliminated:";
	for (int i=1;i<=elicnt;i++){
		if (eli[i]!=0){
			sumx++;
			cout<<" "<<eli[i];
		}
	}
	if (sumx==0) cout<<" None.";
	cout<<"\nUnlocked:";sumx=0;
	for (int i=1;i<=adcnt;i++){
		if (ad[i]!=0){
			sumx++;
			cout<<" "<<ad[i];
		}
	}
	if (sumx==0) cout<<" None.";
	elicnt=adcnt=0;
	memset(eli,0,sizeof(eli));
	memset(ad,0,sizeof(ad));
}
void init(){
	system("color 07");
	cout<<"Show 0?(0=no/1=yes/2=grey):";cin>>show0;
	memset(num,0,sizeof(num));
	s.insert(2);cout<<"Input grid size(2-9):";
	cin>>n;srand(int(time(0)));
}
void genNum(Position pos,int a){
	num[pos.x][pos.y]=a;
}
pair<bool,Position> getRDP(){
	vector<Position> v;
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			if (num[i][j]==0){
				v.push_back(Position(i,j));
			}
		}
	}
	if (v.size()==0){
		Position p;
		return make_pair(false,p);
	}
	return make_pair(true,v[rand()%v.size()]);
}
int getRDPrime(){
	return pr[rand()%prcnt];
}
void rotate(){
	int tempnum[10][10];
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			tempnum[i][j]=num[n-j+1][i];
		}
	}
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			num[i][j]=tempnum[i][j];
		}
	}
}
bool canMerge(int num1,int num2){
	if (num1<=1 || num2<=1) return false;
	else if (num1%num2==0 || num2%num1==0) return true;
	else return false;
}
bool canAct(){
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			if (num[i][j]==0) return true;
		}
	}
	for (int i=1;i<=n;i++){
		for (int j=1;j<n;j++){
			if (canMerge(num[i][j],num[i][j+1])) return true;
		}
	}
	for (int j=1;j<=n;j++){
		for (int i=1;i<n;i++){
			if (canMerge(num[i][j],num[i+1][j])) return true;
		}
	}
	return false;
}
bool act(int way){
	int numtemp[10][10];
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			numtemp[i][j]=num[i][j];
		}
	}
	for (int i=1;i<=way;i++) rotate();
	for (int i=1;i<=n;i++){
		int nums[10]={0};
		int numcnt=0;
		for (int j=1;j<=n;j++){
			if (num[j][i]!=0) nums[++numcnt]=num[j][i];
		}
		for (int j=1;j<numcnt;j++){
			if (canMerge(nums[j],nums[j+1])){
				score+=min(nums[j],nums[j+1]);
				nums[j]=nums[j]+nums[j+1];
				nums[j+1]=0;
			}
		}
		int pos=1;
		for (int j=1;j<=n;j++){
			while (nums[pos]==0 && pos<=numcnt) pos++;
			if (pos>numcnt) num[j][i]=0;
			else{num[j][i]=nums[pos];pos++;}
		}
	}
	for (int i=1;i<=4-way;i++) rotate();
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			if (num[i][j]!=numtemp[i][j]) return true;
		}
	}
	return false;
}
bool isPrime(int nn){
	if (nn<2) return false;
	for (int i=2;i<=sqrt(nn);i++){
		if (nn%i==0) return false;
	}
	return true;
}
vector<int> spl(int nn){
	vector<int>sp;int p=2;
	while (nn>1){
		if (nn%p==0){
			nn/=p;sp.push_back(p);
			continue;
		}
		p++;while (!isPrime(p)) p++;
	}
	return sp;
}
void addPrime(int nn){
	if (nn<2) return;
	vector<int>v=spl(nn);
	for (int i=0;i<(int)v.size();i++){
		if (s.count(v[i])==0){
			s.insert(v[i]);
			prcnt++;pr[prcnt++]=v[i];
			ad[++adcnt]=v[i];
			sort(pr,pr+prcnt);
			prcnt=unique(pr,pr+prcnt)-pr;
		}
	}
}
void PrimeProcess(){
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			addPrime(num[i][j]);
		}
	}
	sort(ad+1,ad+adcnt+1);
	adcnt=unique(ad+1,ad+adcnt+1)-(ad+1);
	for (int p=0;p<prcnt;p++){
		bool flag=false;
		for (int x=1;x<=n;x++){
			for (int y=1;y<=n;y++){
				if (num[x][y]>0){
					vector<int>v=spl(num[x][y]);
					for (int i=0;i<(int)v.size();i++){
						if (v[i]==pr[p]) flag=true;
					}
				}
			}
		}
		if (!flag){
			s.erase(pr[p]);
			eli[++elicnt]=pr[p];
			for (int i=p+1;i<prcnt;i++){
				pr[i-1]=pr[i];
			}
			prcnt--;
		}
	}
	sort(eli+1,eli+elicnt+1);
	elicnt=unique(eli+1,eli+elicnt+1)-(eli+1);
}
int main(){
	init();if (n<2 || n>9) return 0;
	genNum(getRDP().second,getRDPrime());
	genNum(getRDP().second,getRDPrime());
	while (true){
		ClearScreen();
		PrintStatus();
		if (!canAct()) GameOver();
		bool modified=false;
		char ch=getch();
		if (ch=='w' || ch=='W') modified=act(0);
		if (ch=='a' || ch=='A') modified=act(1);
		if (ch=='s' || ch=='S') modified=act(2);
		if (ch=='d' || ch=='D') modified=act(3);
		if (ch=='h' || ch=='H') modified=act(1);
		if (ch=='j' || ch=='J') modified=act(2);
		if (ch=='k' || ch=='K') modified=act(0);
		if (ch=='l' || ch=='L') modified=act(3);
		if ((int)ch==-32){
			char ch=getch();
			if ((int)ch==72) modified=act(0);
			if ((int)ch==75) modified=act(1);
			if ((int)ch==80) modified=act(2);
			if ((int)ch==77) modified=act(3);
		}
		PrimeProcess();
		if (modified){
			auto retval=getRDP();
			if (retval.first==true) genNum(retval.second,getRDPrime());
		}
	}
	return 0;
}