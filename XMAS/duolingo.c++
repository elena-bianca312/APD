#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>


#include <bits/stdc++.h>
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;
int main()
{
    long long n;
    long long x;
    cin >> n >> x;
    int contor = 1;
    int real_contor = 0;

    for (int i = 1; i < n; i++)
    {
        contor = 1;
        unordered_set<int> s;
        queue<int> coada;
        s.insert(0);
        coada.push(i);

        while (!coada.empty())
        {
            int nr = coada.front();
            coada.pop();
            int p = nr * x % n;
            if (s.insert(p).second == true)
            {
                // cout << p << " ";
                coada.push(p);
                contor++;
            }
        }
        if (contor == n)
        {
            real_contor++;
            cout << real_contor;
            return 0;
        }

        cout << endl;
        s.clear();
    }

    // cout << real_contor;
    return 0;
}