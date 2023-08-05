#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
using namespace std;
int show0=0,num[10][10],score,n,playerNow;
bool highlight_merge[10][10],highlight;
int changes[10];
int lastLoser=1,eliminationScore,playercnt,scores[10];
vector<int>v,el,ad;
mt19937 eng;
void gotoxy(short x,short y){
    COORD coord={y,x};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void SetCursorStatus(bool state){
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible=state;
	cursor.dwSize=1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor);
}
void SetFontColor(int ForgC,int BackC){
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
    gotoxy(4,0);
	cout<<"Game Over. Press enter to continue ...\n";
    for (int i=1;i<=playercnt;i++){
        cout<<"Player "<<i<<" has "<<scores[i]<<" pts left. (";
        if (changes[i]>=0){
            SetFontColor(10,0);
            cout<<"+"<<changes[i];
        }
        else{
            SetFontColor(12,0);
            cout<<changes[i];
        }
        SetFontColor(7,0);
        cout<<")\n";
    }
	while (true){
		char ch=getch();
		if (ch=='\n' || ch=='\r') return;
	}
	return;
}
void ClearScreen(){
	gotoxy(0,0);
	cout<<"                             \n                             \n                             \n                             \n                             \n                             \n                             \n                             \n                             \n                             \n                             \n                             \n";
	gotoxy(0,0);
}
int GetLength(int nn){
	if (nn==0) return 1;
	int retval=0;
	while (nn>0){
		nn/=10;
		retval++;
	}
	return retval;
}
int GetMaxLen(){
	int retval=INT_MIN;
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			retval=max(retval,GetLength(num[i][j]));
		}
	}
	return retval;
}
bool IsPrime(int k){
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
	int maxlen=GetMaxLen();
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			if (num[i][j]==0){
				if (show0==2){
					SetFontColor(8,0);
					cout<<setw(maxlen+1)<<num[i][j];
					SetFontColor(7,0);
				}
				else if (show0==1) cout<<setw(maxlen+1)<<num[i][j];
			}
			else{
				if (highlight_merge[i][j] && highlight){
					SetFontColor(14,0);
					cout<<setw(maxlen+1)<<num[i][j];
					SetFontColor(7,0);
				}
				else{
					if (show0==2) SetFontColor(15,0);
					cout<<setw(maxlen+1)<<num[i][j];
					if (show0==2) SetFontColor(7,0);
				}
			}
		}
		cout<<"\n";
	}
	cout<<"Eliminated:";
	for (int i=0;i<(int)el.size();i++){
		cout<<" "<<el[i];
	}
	if (el.size()==0) cout<<" None.";
	cout<<"\nUnlocked:";
	for (int i=0;i<(int)ad.size();i++){
		cout<<" "<<ad[i];
	}
	if (ad.size()==0) cout<<" None.";
	ad.clear();el.clear();cout<<"\n";
    cout<<"Player "<<playerNow<<"'s turn now.";
}
void Init(){
    playerNow=lastLoser;score=0;
	memset(num,0,sizeof(num));
	memset(changes,0,sizeof(changes));
	v.clear();v.emplace_back(2);
	system("cls");
}
void GenNum(Position pos,int a){
	num[pos.x][pos.y]=a;
}
pair<bool,Position> GetRandomPos(){
	vector<Position> v;
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			if (num[i][j]==0){
				v.emplace_back(Position(i,j));
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
int GetRandomPrime(){
	uniform_int_distribution<int>uid(0,v.size()-1);
	return v[uid(eng)];
}
void Rotate(){
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
bool CanMerge(int num1,int num2){
	if (num1<=1 || num2<=1) return false;
	else if (num1%num2==0 || num2%num1==0) return true;
	else return false;
}
void Highlight(){
	memset(highlight_merge,0,sizeof(highlight_merge));
	for (int i=1;i<=n;i++){
		vector<int>v;
		bool flag[10];
		memset(flag,0,sizeof(flag));
		v.clear();
		for (int j=1;j<=n;j++){
			if (num[i][j]>1) v.emplace_back(j);
		}
		if (v.size()>=2){
			for (int j=0;j<(int)v.size()-1;j++){
				if (CanMerge(num[i][v[j]],num[i][v[j+1]])){
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
			if (num[j][i]>1) v.emplace_back(j);
		}
		if (v.size()>=2){
			for (int j=0;j<(int)v.size()-1;j++){
				if (CanMerge(num[v[j]][i],num[v[j+1]][i])){
					flag[v[j]]=flag[v[j+1]]=1;
				}
			}
			for (int j=1;j<=n;j++){
				highlight_merge[j][i]=max(highlight_merge[j][i],flag[j]);
			}
		}
	}
}
bool CanAct(){
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			if (num[i][j]==0) return true;
		}
	}
	for (int i=1;i<=n;i++){
		for (int j=1;j<n;j++){
			if (CanMerge(num[i][j],num[i][j+1])) return true;
		}
	}
	for (int j=1;j<=n;j++){
		for (int i=1;i<n;i++){
			if (CanMerge(num[i][j],num[i+1][j])) return true;
		}
	}
	return false;
}
bool Act(int way){
	int numtemp[10][10];
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			numtemp[i][j]=num[i][j];
		}
	}
	for (int i=1;i<=way;i++) Rotate();
	for (int i=1;i<=n;i++){
		int nums[10]={0};
		int numcnt=0;
		for (int j=1;j<=n;j++){
			if (num[j][i]!=0) nums[++numcnt]=num[j][i];
		}
		for (int j=1;j<numcnt;j++){
			if (CanMerge(nums[j],nums[j+1])){
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
	for (int i=1;i<=4-way;i++) Rotate();
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			if (num[i][j]!=numtemp[i][j]) return true;
		}
	}
	return false;
}
vector<int> Split(int nn){
	vector<int>sp;int p=2;
	while (nn>1){
		if (nn%p==0){
			nn/=p;sp.emplace_back(p);
			continue;
		}
		p++;while (!IsPrime(p)) p++;
	}
	return sp;
}
void AddPrime(int nn){
	if (nn<2) return;
	vector<int>vv=Split(nn);
	for (int i=0;i<(int)vv.size();i++){
		v.emplace_back(vv[i]);
	}
}
void PrimeProcess(){
	vector<int>v_prev=v;v.clear();
	for (int i=1;i<=n;i++){
		for (int j=1;j<=n;j++){
			AddPrime(num[i][j]);
		}
	}
	sort(v.begin(),v.end());
	v.resize(unique(v.begin(),v.end())-v.begin());
	set<int>prev;prev.clear();
	set<int>now;now.clear();
	for (auto k:v_prev) prev.insert(k);
	for (auto k:v) now.insert(k);
	for (auto k:prev){
		if (now.count(k)==0){
			el.emplace_back(k);
		}
	}
	for (auto k:now){
		if (prev.count(k)==0){
			ad.emplace_back(k);
		}
	}
}
bool Play(){
	Init();
	GenNum(GetRandomPos().second,2);
	GenNum(GetRandomPos().second,2);
	while (true){
		ClearScreen();
		Highlight();
		PrintStatus();
		if (!CanAct()){
            scores[playerNow==1? playercnt:playerNow-1]-=score;
			lastLoser=(playerNow==1? playercnt:playerNow-1);
            changes[lastLoser]=-score;
            for (int i=1;i<=playercnt;i++){
                if (i!=lastLoser && scores[i]<0){
                    scores[i]+=50;
                    changes[i]=50;
                }
            }
			GameOver();system("cls");break;
		}
		bool modified=false;
		char ch=getch();
		if (playerNow==1){
			if (ch=='w' || ch=='W') modified=Act(0);
			if (ch=='a' || ch=='A') modified=Act(1);
			if (ch=='s' || ch=='S') modified=Act(2);
			if (ch=='d' || ch=='D') modified=Act(3);
		}
		// player 1
        if (((int)ch==-32 || (int)ch==0)){
            char ch=getch();
            if (playerNow==2){
                if ((int)ch==72) modified=Act(0);
                if ((int)ch==75) modified=Act(1);
                if ((int)ch==80) modified=Act(2);
                if ((int)ch==77) modified=Act(3);
            }
		}
		// player 2
		if (playerNow==3){
			if (ch=='i' || ch=='I') modified=Act(0);
			if (ch=='j' || ch=='J') modified=Act(1);
			if (ch=='k' || ch=='K') modified=Act(2);
			if (ch=='l' || ch=='L') modified=Act(3);
		}
		// player 3
		if (playerNow==4){
			if (ch=='t' || ch=='T') modified=Act(0);
			if (ch=='f' || ch=='F') modified=Act(1);
			if (ch=='g' || ch=='G') modified=Act(2);
			if (ch=='h' || ch=='H') modified=Act(3);
		}
		// player 4
		PrimeProcess();
		if (modified){
			auto retval=GetRandomPos();
			if (retval.first==true) GenNum(retval.second,GetRandomPrime());
            playerNow++;
            // MessageBox(NULL,to_string(playerNow).c_str(),"",MB_OK);
            if (playerNow>playercnt) playerNow=1;
        }
	}
    int sum=0;
    for (int i=1;i<=playercnt;i++){
        if (scores[i]>=0) sum++;
    }
    return sum!=1;
}
int main(){
	system("color 07");score=0;
    eng.seed(time(0));
	cout<<"Show 0?(0=no/1=yes/2=grey):";cin>>show0;
	cout<<"Input grid size(2-9):";cin>>n;
    if (n<2 || n>9) n=4;
	cout<<"Highlight mergable numbers?(0/1):";cin>>highlight;
    cout<<"Elimination Score:";
    cin>>eliminationScore;
    if (eliminationScore<=0) eliminationScore=1500;
    cout<<"Player count(<=4):";
    cin>>playercnt;
    if (playercnt<=1 || playercnt>4) playercnt=3;
    for (int i=1;i<=playercnt;i++) scores[i]=eliminationScore;
	SetCursorStatus(0);
	while (Play());
    system("cls");
    struct Temp{int id,sc;}pl[10];
    for (int i=1;i<=playercnt;i++){
        pl[i].id=i;pl[i].sc=scores[i];
    }
    sort(pl+1,pl+playercnt+1,[=](Temp a,Temp b){return a.sc>b.sc;});
    for (int i=1;i<=playercnt;i++){
        cout<<"Rank "<<i<<": player "<<pl[i].id<<" ("<<pl[i].sc<<"pts)\n";
    }
	return 0;
}