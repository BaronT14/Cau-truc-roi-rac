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
int a[Nmax][Nmax];
int dist[Nmax][Nmax];//khaong cach cua ham ployed
int next1[Nmax][Nmax];//luu dinh ke
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
//nhap ma tran ke co trong so // input : danh sach canh //Ung dung
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

//nhap danh sach ke khong co trong so // input : danh sach canh
void nhapDoThiKhongTrongSo(int& n, int& m) {
	ifstream f;
	f.open("input.txt");
	f >> n >> m;//so dinh, canh
	for (int i = 0; i < m; i++)
	{
		int x, y;//canh dau cuoi
		f >> x >> y;
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
//ployed-warshall
void ployedWarshall(int V) {
	for (int k = 0; k < V; k++) {
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				if (dist[i][j] > dist[i][k] + dist[k][j]) {
					dist[i][j] = dist[i][k] + dist[k][j];
					next1[i][j] = next1[i][k];
				}
			}
		}
	}
}
void printPathUngDung(int s, int dest) {
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
		cout << tmp[i - 1] << " -> ";
		i--;
	}
	cout << den;
	cout << endl;
}
void printPathPloyed(int u, int v) {
	if (next1[u][v] == -1) {
		cout << "Khong co duong di giua " << u << " va " << v << "\n";
		return;
	}
	vector<int> path = { u };
	while (u != v) {
		u = next1[u][v];
		path.push_back(u);
	}
	cout << "Duong di ngan nhat tu " << path[0] << " den  " << v << ": ";
	for (int i = 0; i < path.size(); i++) {
		cout << path[i];
		if (i != path.size() - 1) {
			cout << " -> ";
		}
	}
	cout << "\n";
}
void printPath(int s, int dest) {
	if (khoangCach[dest] == INF) {
		cout << "Khong co duong nao tu " << s << " den " << dest << endl;
		return;
	}

	cout << "Duong ngan nhat tu " << s << " den " << dest << ": ";
	int current = dest;
	int tmp[Nmax]; int i = 0;
	int den = current;
	while (pre[current].ma != INF) {
		tmp[i] = pre[current].ma;
		current = pre[current].ma;
		i++;
	}
	while (i != 0)
	{
		cout << tmp[i - 1] << " -> ";
		i--;
	}
	cout << den;
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

int maVung(string s, int n) {
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
//Xuat danh sach ke cua 1 dinh
void printAdjList(int i) {
	cout << "Danh sach ke cua dinh " << i << ": ";
	tham[i] = false;
	for (int x : adj[i]) {
		if (!tham[x]) {
			cout << x << " ";
			tham[x] = true;
		}
	}
	cout << endl;
}
// Hàm xuất tất cả danh sách kề
void printAdjacencyList(int n) {
	cout << "Danh sach ke cua do thi:" << endl;
	for (int i = 1; i <= n; ++i) {
		cout << "Dinh " << i << ": ";
		for (int x : adj[i]) {
			cout << x << " ";
		}
		cout << endl;
	}
}
//chuyen doi danh sach ke thanh ma tran ke
void DSKtoMTK(int n) {
	for (int i = 1; i <= n; i++) {
		for (int x : adj[i]) {
			a[i][x] = 1;
			a[x][i] = 1;
		}
	}
}
//xuat ma tran ke
void printGraph(int n) {
	DSKtoMTK(n);
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			cout << setw(5) << a[i][j];
		}
		cout << endl;
	}
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
	cout << "1 . Duyet BFS" << endl;
	cout << "2 . Duyet DFS" << endl;
	cout << "3 . Tinh lien thong cua do thi" << endl;
	cout << "4 . Duong di ngan nhat (thuat toan dijkstra)" << endl;
	cout << "5 . Xuat ma tran ke " << endl;
	cout << "6 . Tim dinh ke voi dinh i" << endl;
	cout << "7 . Xuat danh sach ke" << endl;
	cout << "8 . Duong di ngan nhat <Thuat toan Ployed-Warshall>" << endl;
	cout << "--------------------------UNG DUNG--------------------------" << endl;
	cout << "9 . Chon tuyen xe ngan nhat" << endl;
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
			int j;
			cout << "Duyet tu dinh: "; cin >> j;
			nhapDoThiKhongTrongSo(n, m);
			BFS(j);
			for (int x : temp) {
				cout << setw(5) << x;
			}
			cout << endl;
		}break;
		case 2: {
			int j;
			cout << "Duyet tu dinh: "; cin >> j;
			nhapDoThiKhongTrongSo(n, m);
			DFS(j);
			for (int x : temp) {
				cout << setw(5) << x;
			}
			cout << endl;
		}break;
		case 3: {
			nhapDoThiKhongTrongSo(n, m);
			int cnt = ktrTinhLienThong(n, m);
			if (cnt == 1)
				cout << "Do thi lien thong" << endl;
			else {
				cout << "Do thi khong lien thong" << endl;
				cout << "Co " << cnt << " do thi con lien thong" << endl;
			}
		}break;
		case 4: {
			int j;
			cout << "Nhap dinh bat dau: "; cin >> j;
			nhapDoThiTrongSo(a, n, m);
			dijkstra(j, n);
			for (int i = 0; i < n; i++)
			{
				printPath(j, i);
				cout << "| Khoang cach : " << khoangCach[i] << endl;
			}
		}break;
		case 5: {//biểu diễn bằng ma trận kề
			nhapDoThiKhongTrongSo(n, m);
			printGraph(n);
		}break;
		case 6: {
			// Hàm xuất danh sách kề của đỉnh i
			int i;
			cout << "Nhap dinh i: "; cin >> i;
			nhapDoThiKhongTrongSo(n, m);
			printAdjList(i);
		}break;
		case 7: {
			nhapDoThiKhongTrongSo(n, m);
			printAdjacencyList(n);
		}break;
		case 8: {
			nhapDoThiTrongSo(a, n, m);
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					if (a[i][j]) {
						dist[i][j] = a[i][j];
						next1[i][j] = j;
					}
					else {
						dist[i][j] = INF;
						next1[i][j] = -1;
					}
				}
			}
			ployedWarshall(n);
			int start, end;
			cout << "Nhap 2 diem can tim duong di ngan nhat: ";
			cin >> start >> end;
			printPathPloyed(start, end);

		}break;
		case 9: {
			nhapDoThiTrongSo(a, n, m);
			cout << "Nhap noi bat xe: ";
			string di, den;
			cin.ignore();
			getline(cin, di);
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
			printPathUngDung(x, y);
			cout << "Tong duong di tu " << di << " -> " << den << ": " << khoangCach[y] * 10 << "km" << endl;
		}break;
		default:
			break;
		}
		system("pause");
		system("cls");
	} while (c != 0);

	_getch();
	return 0;
}