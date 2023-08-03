#include<bits/stdc++.h>
#include<windows.h>
#include<wchar.h>
#include<conio.h>
#define int long long
using namespace std;
int show0,num[10][10],score,scoreprev,n,col;
bool llexpl=false;
mt19937 eng;
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
string toStr(int num){
	if (num>65536){
		if (log2(num)-floor(log2(num))<1e-4){
			string retval="^";
			return retval+to_string(signed(floor(log2(num))));
		}
	}
	switch (num){
		case -2:return "O3";
		case -1:return "O2";
		case 1:return "#";
		case 2:return "CE";
		case 4:return "Jud";
		case 8:return "RE";
		case 16:return "TLE";
		case 32:return "MLE";
		case 64:return "ILE";
		case 128:return "OLE";
		case 256:return "UKE";
		case 512:return "WA";
		case 1024:return "PC";
		case 2048:return "AC";
		case 4096:return "PE";
		case 8192:return "DoJ";
		case 16384:return "SJE";
		case 32768:return "AU";
		case 65536:return "AK";
		default:return "nop";
	}
	return "???";
}
int getColor(int num){
	switch (num){
		case -2:return 15;
		case -1:return 15;
		case 1:return 15;
		case 2:return 14;
		case 4:return 11;
		case 8:return 5;
		case 16:return 9;
		case 32:return 9;
		case 64:return 9;
		case 128:return 9;
		case 256:return 8;
		case 512:return 12;
		case 1024:return 14;
		case 2048:return 10;
		case 4096:return 13;
		case 8192:return 7;
		case 16384:return 8;
		case 32768:return 14;
		case 65536:return 11;
		default:return 15;
	}
	return 0;
}
void PrintStatus(){
	if (llexpl) cout<<"Score: OVERFLOW\n";
	else cout<<"Score: "<<score<<"\n";
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			if (num[i][j]==0){
				if (show0==2){
					setFontColor(8,0);
					cout<<" nop";
					setFontColor(7,0);
				}
				else if (show0==1) cout<<setw(4)<<toStr(num[i][j]);
			}
			else{
				if (show0==2 && !col) setFontColor(15,0);
				else if (col) setFontColor(getColor(num[i][j]),0);
				cout<<setw(4)<<toStr(num[i][j]);
				setFontColor(7,0);
			}
		}
		cout<<"\n";
	}
}
void init(){
	system("color 07");
	cout<<"Show 0 or not?(0=n/1=y/2=grey):";cin>>show0;
	memset(num,0,sizeof(num));
	cout<<"Input grid size(2-9):";
	cin>>n;cout<<"Colored or not?(0/1)";
	cin>>col;eng.seed(time(0));
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
	if (num1==num2 && num1!=-2) return true;
    else if (num1==1 || num2==1) return true;
	else if (num1==-1 && num2!=-2) return true;
	else if (num2==-1 && num1!=-2) return true;
	else if (num1==-2 && num2>=2) return true;
	else if (num2==-2 && num1>=2) return true;
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
		int numcnt=0,maxpos=INT_MAX;
		for (int j=1;j<=n;j++){
			if (num[j][i]==0 && maxpos==INT_MAX) maxpos=j;
			if (num[j][i]!=0) nums[++numcnt]=num[j][i];
		}
		for (int j=1;j<numcnt;j++){
			if (canMerge(nums[j],nums[j+1])){
                if (nums[j]==1 && nums[j+1]==1){//Two #s are included
                    nums[j]=-1;nums[j+1]=INT_MIN;
                }
				else if (nums[j]==1 || nums[j+1]==1){//One # is included
					if (j+1<maxpos){
						if (nums[j]==1){
							if (nums[j+1]>=2){
								score+=nums[j+1];
								nums[j]=nums[j+1];
								nums[j+1]=INT_MIN;
							}
							else{
								nums[j]=nums[j+1];
								nums[j+1]=INT_MIN;
							}
						}
						else{
							if (nums[j]>=2){
								score+=nums[j];
								nums[j+1]=INT_MIN;
							}
							else{
								nums[j+1]=INT_MIN;
							}
						}
					}
				}
				else{//No # are included
					if (nums[j]==-1 && nums[j+1]==-1){//Two O2s are included
						nums[j]=-2;
						nums[j+1]=INT_MIN;
					}
					else if (nums[j]==-1 || nums[j+1]==-1){//One O2 is included
						if (j+1<maxpos){
							nums[j]=max(nums[j],nums[j+1])*2;
							nums[j+1]=INT_MIN;
							score+=nums[j];
						}
					}
					else{//No O2 are included
						if (nums[j]==-2 || nums[j+1]==-2){//One O3 is included
							nums[j]=max(nums[j],nums[j+1])*4;
							nums[j+1]=INT_MIN;
							score+=nums[j];
						}
						else{//Two common numbers
							nums[j]+=nums[j+1];
							nums[j+1]=INT_MIN;
							score+=nums[j];
						}
					}
				}
			}
		}
		int pos=1;
		for (int j=1;j<=n;j++){
			while (nums[pos]==INT_MIN && pos<=numcnt) pos++;
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
int getRDNum(){
	uniform_int_distribution<int>uid(1,100);
    int a=uid(eng);
    if (a<=70) return 2;
    if (a<=90) return 4;
    if (a<=97) return 1;
    else return -1;
}
signed main(){
	init();if (n<2 || n>9) return 0;
	genNum(getRDP().second,2);
	genNum(getRDP().second,2);
	while (true){
		ClearScreen();
		PrintStatus();
		if (!canAct()) GameOver();
		bool modified=false;
		scoreprev=score;
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
		if (score<scoreprev){
			llexpl=true;
		}
		if (modified){
			auto retval=getRDP();
			if (retval.first==true) genNum(retval.second,getRDNum());
		}
	}
	return 0;
}
