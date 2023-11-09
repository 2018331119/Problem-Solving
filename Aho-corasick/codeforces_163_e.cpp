// https://codeforces.com/problemset/problem/163/E
#include<bits/stdc++.h>
using namespace std;
struct AC {
  	int N, P;
  	const int A = 26;
  	vector <vector <int>> next;
  	vector <int> link;
  	vector <vector <int>> out;
  	AC(): N(0), P(0) {node();}
  	int node() {
		next.emplace_back(A, 0);
		link.emplace_back(0);
		return N++;
  	}
  	inline int get (char c) {
		return c - 'a';
  	}
  	int add_pattern (const string T) {
		int u = 0;
		for (auto c : T) {
  			if (!next[u][get(c)]) next[u][get(c)] = node();
  			u = next[u][get(c)];
		}
		return u;
  	}
  	void compute() {
		queue <int> q;
		for (q.push(0); !q.empty();) {
	  		int u = q.front(); q.pop();
	  		for (int c = 0; c < A; ++c) {
				int v = next[u][c];
				if (!v) next[u][c] = next[link[u]][c];
				else {
  					link[v] = u ? next[link[u]][c] : 0;
  					q.push(v);
				}
			}
		}
	}
  	int advance (int u, char c) {
		while (u && !next[u][get(c)]) u = link[u];
		u = next[u][get(c)];
		return u;
  	}
};
#define ll long long int
struct BIT{
	int n;
	std::vector<ll> bt;
	BIT() {}
	BIT(int _n) {
		n = _n;
		bt.resize(n + 1);
	}
	void update(int i, ll val) {
		if(i <= 0) return;
		while(i <= n) {
			bt[i] += val;
			i += i&(-i);
		}
	}
	void update(int i, int j, ll val) {
			if(i > j) return;
			update(i, val);
			update(j + 1, -val);
	}
	ll query(int l) {
		ll ans = 0;
		while(l > 0) {
			ans += bt[l];
			l -= l&-l;
		}
		return ans;
	}
	ll query(int l, int r) {
		if(l > r || l == 0) return 0LL;
		return query(r) - query(l - 1);
	}
};
std::vector<vector<int>>adj;
std::vector<int> tin, tout;
int tim;
void dfs(int node) {
	tin[node] = ++tim;
	for(int &u:adj[node])
		dfs(u);
	tout[node] = tim;
}
int32_t main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	int n, k; cin >> n >> k;
	AC aho;
	std::vector<int> id(k);
	for(int i = 0; i < k; i++) {
		string c; cin >> c;
		id[i] = aho.add_pattern(c);
	}
	aho.compute();
	int m = aho.N;
	adj.resize(m);
	tin.resize(m);
	tout.resize(m);
	for(int i = 1; i < m; i++)
		adj[aho.link[i]].push_back(i);	
	dfs(0);
	BIT bit(m);
	std::vector<int> ase(k, 1);
	for(int i = 0; i < k; i++) {
		bit.update(tin[id[i]], 1);
		bit.update(tout[id[i]] + 1, -1);
	}
	auto solve = [&](string c) {
		ll ans = 0;
		int crt = 0;
		for(char &cc:c) {
			crt = aho.advance(crt, cc);
			ans += bit.query(tin[crt]);
		}
		return ans;
	};
	while(n--) {
		char c; cin >> c;
		if(c == '?') {
			string d; cin >> d;
			cout << solve(d) << "\n";
		} else if(c == '-') {
			int a; cin >> a;
			a--;
			if(!ase[a]) continue;
			ase[a] = 0;
			bit.update(tin[id[a]], -1);
			bit.update(tout[id[a]] + 1, 1);
		} else {
			int a; cin >> a;
			a--;
			if(ase[a]) continue;
			ase[a] = 1;
			bit.update(tin[id[a]], 1);
			bit.update(tout[id[a]] + 1, -1);
		}
	}
	return 0; 
}
