#include <iostream>
#include <map>
#include <vector>

using namespace std;

#define NUMS 6
#define SUBSETS (1<<NUMS)

struct Way
{
    char oper;
    int arg1, arg2;
    int subset1, subset2;
};

vector<map<int, Way> > maps;

int ones(int n)
{
    if (n==0) {
        return 0;
    }

    if (n&1) {
        return 1 + ones(n>>1);
    }

    return ones(n>>1);
}

void add(map<int, Way> &m, Way w)
{
    int result;

    switch (w.oper) {
    case ' ':
        result = w.arg1;

        break;

    case '+':
        result = w.arg1+w.arg2;

        break;

    case '-':
        if (w.arg1<w.arg2 || maps[w.subset2][w.arg2].oper=='-') {
            return;
        }

        result = w.arg1-w.arg2;

        break;

    case '*':

        if (w.arg1==1 || w.arg2==1) {
            return;
        }

        result = w.arg1*w.arg2;

        break;

    case '/':
        if (w.arg2==0 || (w.arg1/w.arg2)*w.arg2!=w.arg1) {
            return;
        }

        result = w.arg1/w.arg2;

        break;
    }

    if (m.find(result)==m.end()) {
        m[result] = w;
    }
}

void print(int goal, int s)
{
    Way w = maps[s][goal];

    if (w.oper==' ') {
        cout << goal;

        return;
    }

    bool p1, p2;
    char o1 = maps[w.subset1][w.arg1].oper;
    char o2 = maps[w.subset2][w.arg2].oper;

    p1 = p2 = w.oper == '*' || w.oper == '/';
    p1 &= o1 == '+' || o1 == '-';
    p2 &= o2 == '+' || o2 == '-';

    if (p1) {
        cout << "(";
    }

    print(w.arg1, w.subset1);

    if (p1) {
        cout << ")";
    }

    cout << w.oper;

    if (p2) {
        cout << "(";
    }

    print(w.arg2, w.subset2);

    if (p2) {
        cout << ")";
    }
}

int main()
{
    int goal = 434;
    int nums[NUMS] = {4, 5, 2, 1, 20, 100};

    cout << "Enter desired number : " << flush;
    cin >> goal;
    cout << "Enter " << NUMS << " numbers : " << flush;

    for (int i = 0; i < NUMS; ++i) {
        cin >> nums[i];
    }

    cout << "Desired number is : " << goal;
    cout << "\nOffered numbers are :";

    for (int i = 0; i < NUMS; ++i) {
        cout << " " << nums[i];
    }

    cout << "\nSolution is : ";
    maps.resize(SUBSETS);

    for (int o = 1; o <= NUMS; ++o) {
        for (int s = 1; s < SUBSETS; ++s) {
            if (ones(s)!=o) {
                continue;
            }

            for (int i = 0; i < NUMS; ++i) {
                if (s&(1<<i)) {
                    Way w;

                    w.arg1 = nums[i];
                    w.arg2 = 0;
                    w.oper = ' ';
                    w.subset1 = s;
                    w.subset2 = 0;
                    maps[s][w.arg1] = w;
                }
            }

            for (int s1=1; s1<SUBSETS; ++s1) {
                if (s==s1 || (s&s1)!=s1) {
                    continue;
                }

                int s2 = s^s1;

                map<int, Way> &m = maps[s];
                map<int, Way> &m1 = maps[s1];
                map<int, Way> &m2 = maps[s2];

                for (map<int, Way>::const_iterator i1 = m1.begin(); i1!=m1.end(); ++i1) {
                    for (map<int, Way>::const_iterator i2 = m2.begin(); i2!=m2.end(); ++i2) {
                        Way w;

                        w.arg1 = i1->first;
                        w.arg2 = i2->first;
                        w.subset1 = s1;
                        w.subset2 = s2;

                        char op[] = " +-*/";

                        for (int i = 0; i < 5; ++i) {
                            w.oper = op[i];
                            add(m, w);
                        }
                    }
                }
            }
        }
    }

    int min = -1;
    map<int, Way> &m = maps[SUBSETS-1];

    for (map<int, Way>::iterator i = m.begin(); i != m.end(); ++i) {
        int d = (i->first) - goal;

        if (d<0) {
            d = -d;
        }

        if (min==-1 || d<min) {
            min = d;
        }
    }

    for (map<int, Way>::iterator i = m.begin(); i != m.end(); ++i) {
        int d = (i->first) - goal;

        if (d<0) {
            d = -d;
        }

        if (d!=min) {
            continue;
        }

        print(i->first, SUBSETS-1);

        break;
    }

    cout << endl;

    return 0;
}
