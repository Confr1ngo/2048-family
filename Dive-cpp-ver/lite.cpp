#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
using namespace std;
int show0=0,num[10][10],score,n;
bool highlight_merge[10][10],highlight;
vector<int>v;
mt19937 eng;
void gotoxy(short x,short y){
    COORD coord={y,x};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void setCursorStatus(bool state){
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible=state;
	cursor.dwSize=1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor);
}
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
	gotoxy(0,0);
	cout<<"                             \n                             \n                             \n                             \n                             \n                             \n                             \n                             \n                             \n                             \n";
	gotoxy(0,0);
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
bool isPrime(int k){
	if (k<2) return false;
	if (k==2) return true;
	for (int i=2;i<=sqrt(k);i++){
		if (!(k%i)) return false;
	}
	return true;
}
void PrintStatus(){
	cout<<"Seeds:";
	for (auto i:v) cout<<" "<<i;
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
				if (highlight_merge[i][j] && highlight){
					setFontColor(14,0);
					cout<<setw(maxlen+1)<<num[i][j];
					setFontColor(7,0);
				}
				else{
					if (show0==2) setFontColor(15,0);
					cout<<setw(maxlen+1)<<num[i][j];
					if (show0==2) setFontColor(7,0);
				}
			}
		}
		cout<<"\n";
	}
}
void init(){
	system("color 07");
	cout<<"Show 0?(0=no/1=yes/2=grey):";cin>>show0;
	memset(num,0,sizeof(num));
	v.emplace_back(2);cout<<"Input grid size(2-9):";
	cin>>n;eng.seed(time(0));
	cout<<"Highlight mergable numbers?(0/1):";
	cin>>highlight;
	system("cls");
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
	uniform_int_distribution<int>uid(0,v.size()-1);
	return make_pair(true,v[uid(eng)]);
}
int getRDPrime(){
	uniform_int_distribution<int>uid(0,v.size()-1);
	return v[uid(eng)];
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
void highlightIt(){
	memset(highlight_merge,0,sizeof(highlight_merge));
	for (int i=1;i<=n;i++){
		vector<int>v;
		bool flag[10];
		memset(flag,0,sizeof(flag));
		v.clear();
		for (int j=1;j<=n;j++){
			if (num[i][j]>1) v.push_back(j);
		}
		if (v.size()>=2){
			for (int j=0;j<(int)v.size()-1;j++){
				if (canMerge(num[i][v[j]],num[i][v[j+1]])){
					flag[v[j]]=flag[v[j+1]]=1;
				}
			}
			for (int j=1;j<=n;j++){
				highlight_merge[i][j]=max(highlight_merge[i][j],flag[j]);
			}
		}
	}
	for (int i=1;i<=n;i++){
		vector<int>v;
		bool flag[10];
		memset(flag,0,sizeof(flag));
		v.clear();
		for (int j=1;j<=n;j++){
			if (num[j][i]>1) v.push_back(j);
		}
		if (v.size()>=2){
			for (int j=0;j<(int)v.size()-1;j++){
				if (canMerge(num[v[j]][i],num[v[j+1]][i])){
					flag[v[j]]=flag[v[j+1]]=1;
				}
			}
			for (int j=1;j<=n;j++){
				highlight_merge[j][i]=max(highlight_merge[j][i],flag[j]);
			}
		}
	}
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
	vector<int>vv=spl(nn);
	for (int i=0;i<(int)vv.size();i++){
		v.push_back(vv[i]);
	}
}
void PrimeProcess(){
	vector<int>v_prev=v;v.clear();
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			addPrime(num[i][j]);
		}
	}
	sort(v.begin(),v.end());
	v.resize(unique(v.begin(),v.end())-v.begin());
}
int main(){
	setCursorStatus(0);
	init();if (n<2 || n>9) return 0;
	genNum(getRDP().second,2);
	genNum(getRDP().second,2);
	while (true){
		ClearScreen();
		highlightIt();
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
		if ((int)ch==-32 || (int)ch==0){
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