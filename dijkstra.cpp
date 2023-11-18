#include<iostream>
#include<conio.h>
#include<fstream>
#include<iomanip>
#include<string>
#include<queue>
using namespace std;

const int Nmax = 100;
const int INF = 1e9; //infinity

string tenVung(int s, int n);

struct canh
{
	int dau, cuoi, w;
};
struct ma_Vung
{
	string ten;
	int ma;
};
int next1[Nmax][Nmax];
int a[Nmax][Nmax];
int khoangCach[Nmax]; // khoang cach ngan nhat toi i
bool tham[Nmax];
ma_Vung pre[Nmax]; // lưu đỉnh trc đó trên đường đi ngắn nhất từ đỉnh nguồn tới mỗi đỉnh
ma_Vung maVungArr[Nmax];
vector<int> adj[Nmax]; // bieu dien do thi bang danh sach ke
vector<int> temp;

void khoiTao(int a[][Nmax]) {
	for (int i = 0; i < Nmax; i++)
	{
		for (int j = 0; j < Nmax; j++)
		{
			a[i][j] = 0;
		}
		khoangCach[i] = INF;
		tham[i] = false;
		pre[i].ma = INF;
	}
	temp.clear();
}
//nhap bang danh sach canh co trong so // bieu dien bang ma tren ke
void nhapDoThiTrongSo(int a[][Nmax], int& n, int& m) {
	ifstream f;
	f.open("inputDijkstra.txt");
	f >> n >> m;//so dinh, canh
	for (int i = 0; i < m; i++)
	{
		int x, y, w;//canh dau cuoi trong so
		f >> x >> y >> w;
		a[x][y] = a[y][x] = w;
	}
	f.close();
}

//nhap bang danh sach canh khong co trong so //bieu dien bang danh sach ke
void nhapDoThiKhongTrongSo(int& n, int& m) { 
	ifstream f;
	f.open("input.txt");
	f >> n >> m;//so dinh, canh
	for (int i = 0; i < m; i++)
	{
		int x, y;//canh dau cuoi
		f >> x >> y ;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}
	f.close();
}

 //thuat toan dijkstra  matranke
void dijkstra(int s, int n) {
	khoangCach[s] = 0;
	for (int i = 0; i < n; i++)
	{
		//Chon dinh chua tham co khoang cach toi nguon ngan nhat
		int u = -1;//Dinh kc ngan nhat
		for (int j = 0; j < n; j++)
		{
			if (!tham[j] && (u == -1 || khoangCach[j] < khoangCach[u]))
			{
				u = j;
			}
		}
		tham[u] = true;

		//cap nhat khoang cach tu dinh nguon toi dinh ke cua u
		for (int v = 0; v < n; v++)
		{
			if (!tham[v] && a[u][v] && khoangCach[u] != INF && khoangCach[u] + a[u][v] < khoangCach[v])
			{
				khoangCach[v] = khoangCach[u] + a[u][v];
				pre[v].ma = u;
				string tmp = tenVung(u, n);
				pre[v].ten = tmp;
			}
		}
	}
}

void printPath(int s, int dest) {
	if (khoangCach[dest] == INF) {
		cout << "Khong co duong nao tu " << maVungArr[s].ten << " den " << maVungArr[dest].ten << endl;
		return;
	}

	cout << "Duong ngan nhat tu " << maVungArr[s].ten << " den " << maVungArr[dest].ten << ": ";
	ma_Vung current = maVungArr[dest];
	string tmp[Nmax]; int i = 0;
	string den = current.ten;
	while (pre[current.ma].ma != INF) {
		tmp[i] = pre[current.ma].ten;
		current = pre[current.ma];
		i++;
	}
	while (i != 0)
	{
		cout << tmp[i-1] << " -> ";
		i--;
	}
	cout << den;
	cout << endl;
}
void inputMaVung() {
	ifstream f;
	f.open("inputmaVung.txt");
	int i = 0;
	ma_Vung tmp;
	while (!f.eof())
	{
		f.ignore();
		getline(f, tmp.ten);
		f >> tmp.ma;
		maVungArr[i] = tmp;
		i++;
	}
	f.close();
}

int maVung(string s, int n){
	for (int i = 0; i < n; i++)
	{
		if (maVungArr[i].ten == s)
		{
			return maVungArr[i].ma;
		}
	}
	return -1;
}
string tenVung(int s, int n) {
	for (int i = 0; i < n; i++)
	{
		if (maVungArr[i].ma == s)
		{
			return maVungArr[i].ten;
		}
	}
	return "-1";
}

//thuat toan floyd
void floydWarshall(int a[][Nmax], int n) {
	memset(next1, -1, sizeof next1);
	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= n; j++) {
			if (a[i][j] != INF && a[i][j] != 0) { // Co duong di giua i va j
				next1[i][j] = j;
			}
		}
	}
	for (int k = 1; k < n; k++)
		for (int i = 1; i < n; i++)
			for (int j = 1; j < n; j++)
				if (a[i][k] != INF && a[k][j] != INF && a[i][k] + a[k][j] < a[i][j]) {
					a[i][j] = a[i][k] + a[k][j];
					next1[i][j] = next1[i][k];
				}
	cout << "hello\n";
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (a[i][j] == INF) {
				cout << "Khong co duong di tu " << i << " toi " << j << endl;
				continue;
			}
			else if (i != j) {
				cout << "Duong " << i << " toi " << j << ": ";
				for (int tmp = i; tmp !=j ; tmp = next1[tmp][j]) {
					cout << tmp << " ";
				}
				cout << j << endl;
			}
		}
	}
}
//duyet thuat toan BFS  Danhsachke
void BFS(int u) {
	queue<int> q; 
	q.push(u);
	tham[u] = true;
	while (!q.empty())
	{
		int v = q.front();
		q.pop();
		temp.push_back(v);
		for (int x : adj[v])
		{
			if (tham[x] == false)
			{
				q.push(x);
				tham[x] = true;
			}
		}
	}
}
//duyet thuat toan DFS  Danhsachke
void DFS(int u) {
	temp.push_back(u);
	tham[u] = true;
	for (int v : adj[u])
	{
		if (!tham[v])
			DFS(v);
	}
}
//ktr tính lien thong
int ktrTinhLienThong(int n, int m) {
	int cnt = 0;
	for (int i = 1; i <= n; i++)
	{
		if (!tham[i]) {
			++cnt;
			BFS(i);
		}
	}
	return cnt;
}

void chuanHoa(string& s) {
	int n = s.length();
	for (int i = 0; i < n; i++) {
		if (s[i] == ' ') {
			for (int j = i; j < n - 1; j++) {
				s[j] = s[j + 1];
			}
			s[n - 1] = NULL;
			i--;
			n--;
		}
		else break;
	}
	for (int i = n - 1; i >= 0; i--) {
		if (s[i] == ' ') {
			s[i] = NULL;
			n--;
		}
		else break;
	}
	for (int i = 1; i < n - 1; i++)
	{
		if (s[i] == s[i + 1] && s[i] == ' ') {
			for (int j = i; j < n - 1; j++) {
				s[j] = s[j + 1];
			}
			s[n - 1] = NULL;
			i--;
			n--;
		}
	}
	s.resize(n);
	for (int i = 0; i < n; i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] = s[i] + 32;
		}
	}
	s[0] = s[0] - 32;
	for (int i = 1; i < n; i++)
	{
		if (s[i - 1] == ' ')
		{
			s[i] -= 32;
		}
	}
}

void menu(int& c) {
	cout << "----------------------------MENU----------------------------" << endl;
	cout << "2 . Duyet BFS" << endl;
	cout << "3 . Duyet DFS" << endl;
	cout << "4 . Tinh lien thong cua do thi" << endl;
	cout << "5 . Duong di ngan nhat (thuat toan dijkstra)" << endl;
	cout << "6 . Duong di ngan nhat (thuat toan ployed warshall)" << endl;
	cout << "--------------------------UNG DUNG--------------------------" << endl;
	cout << "1 . Chon tuyen xe ngan nhat" << endl;
	cout << "0 . Thoat" << endl;
	cout << "------------------------------------------------------------" << endl;
	cout << "Nhap lua chon: "; cin >> c;
}

int main() {
	int n, m, c;
	inputMaVung();
	do
	{
		khoiTao(a);
		menu(c);
		switch (c)
		{
		case 1: {
			nhapDoThiTrongSo(a, n, m);
			cout << "Nhap noi bat xe: ";
			string di, den;
			cin.ignore();
			getline(cin,di);
			chuanHoa(di);
			cout << "Nhap diem den: ";
			getline(cin, den);
			chuanHoa(den);
			int x = maVung(di, n);
			int y = maVung(den, n);
			if (x == -1 || y == -1) {
				cout << "Khong co tuyen xe theo nhu yeu cau" << endl;
				break;
			}
			dijkstra(x, n);
			printPath(x,y);
			cout << "Tong duong di tu " << di << " -> " << den << ": " << khoangCach[y] * 10 << "km" << endl;
		}break;
		case 2: {
			int j;
			cout << "Duyet tu dinh: "; cin >> j;
			nhapDoThiKhongTrongSo(n, m);
			BFS(j);
			for (int x : temp) {
				cout << setw(5) << x;
			}
			cout << endl;
		}break;
		case 3: {
			int j;
			cout << "Duyet tu dinh: "; cin >> j;
			nhapDoThiKhongTrongSo(n, m);
			DFS(j);
			for (int x : temp) {
				cout << setw(5) << x;
			}
			cout << endl;
		}break;
		case 4: {
			nhapDoThiKhongTrongSo(n, m);
			int cnt = ktrTinhLienThong(n, m);
			if (cnt == 1)
				cout << "Do thi lien thong" << endl;
			else {
				cout << "Do thi khong lien thong" << endl;
				cout << "Co " << cnt << " do thi con lien thong" << endl;
			}
		}break;
		case 5: {
			int j;
			cout << "Nhap dinh bat dau: "; cin >> j;
			nhapDoThiKhongTrongSo(n, m);
			dijkstra(j, n);
			for (int i = 0; i < n; i++)
			{
				if (j == i) {
					cout << j;
				}

				int current = i;
				cout << "Duong ngan nhat tu " << j << " den " << i << ": " << current;
				while (current != j) {
					cout << " <- " << pre[current].ma;
					current = pre[current].ma;
				}
				cout << endl;
			}

		}break;
		case 6: {
			nhapDoThiTrongSo(a, n, m);
			floydWarshall(a, n);
			// In ma trận khoảng cách
    cout << "Ma tran khoang cach sau khi thuc hien Floyd-Warshall: \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(5) << a[i][j];
        }
        cout << endl;
    }
	
    // Gọi hàm in đường đi ngắn nhất từ đỉnh 0 đến đỉnh 1

		}break;
		default:
			break;
		}
		system("pause");
		system("cls");
	} while (c!=0);


	//duyet do thi
	/*for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << setw(5) << a[i][j];
		}
		cout << endl;
	}*/

	_getch();
	return 0;
}