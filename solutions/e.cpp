#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <stack>
#include <string>
#include <bitset>
#include <ctime>
#include <random>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <complex>
#include <assert.h>
#include <chrono>
#pragma comment(linker, "/STACK:256000000")
using namespace std;

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#pragma warning(disable : 4996)

#define in(s) freopen(s, "r", stdin)
#define out(s) freopen(s, "w", stdout)
#define forn(i, n) for (int i = 0; i < n; i++)
#define mp make_pair

typedef long long ll;
typedef double ld;

struct project {
    string name;
    int len, score, dead;
    vector< pair<string, int> > req;
};

int t = 0;

bool cmp(const project& a, const project& b) {
    int ca = 1.0 * max(0, a.score - max(0, t + a.len - a.dead)) / (a.len / 5) / a.req.size();
    int cb = 1.0 * max(0, b.score - max(0, t + b.len - b.dead)) / (b.len / 5) / b.req.size();
    return ca > cb;
}


int main()
{
    in("e_exceptional_skills.in.txt");
    out("eoutput.txt");
    srand(1373737);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n, m;
    cin >> n >> m;

    unordered_map<string, unordered_map<string, int> > skills;
    unordered_map<string, vector<string>> skillusers;

    forn(i, n) {
        int x, lvl;
        string name, skill;
        cin >> name >> x;
        while (x--) {
            cin >> skill >> lvl;
            skills[name][skill] = lvl;
            skillusers[skill].push_back(name);
        }
    }

    int maxdead = -1;

    vector<project> projects(m);
    forn(i, m) {
        cin >> projects[i].name >> projects[i].len >> projects[i].score >> projects[i].dead;
        maxdead = max(maxdead, projects[i].dead);
        int r;
        cin >> r;
        projects[i].req.resize(r);
        forn(j, r) {
            string skill;
            int lvl;
            cin >> skill >> lvl;
            projects[i].req[j] = mp(skill, lvl);
        }
    }

    maxdead = 100 * maxdead;

    unordered_map<string, int> busy;
    vector<pair<string, vector<string>>> sol;


    while (t <= maxdead) {
        cerr << t << ' ' << maxdead << ' ' << projects.size() << std::endl;
        sort(projects.begin(), projects.end(), cmp);

        bool any = false;
        bool anyCost = false;

        forn(i, (int)projects.size()) {
            if ((projects[i].score - max(0, t + projects[i].len - projects[i].dead)) > 0) anyCost = true;
            else continue;
            vector<string> match(projects[i].req.size());
            set<string> used;
            bool ok = true;

            unordered_map<string, int> maxskill;

            forn(j, (int)projects[i].req.size()) {
                string nskill = projects[i].req[j].first;
                int nlvl = projects[i].req[j].second;
                if (maxskill[nskill] >= nlvl)
                    nlvl--;

                int minu = (int)1e9;
                string minun = "";

                for (auto it = skillusers[nskill].begin(); it != skillusers[nskill].end(); it++) {
                    string user = *it;
                    int hlvl = skills[user][nskill];

                    if (hlvl >= nlvl && busy[user] <= t && !used.count(user)) {
                        if (minu >= hlvl) {
                            minu = hlvl;
                            minun = user;
                        }
                    }
                }

                if (minun == "") {
                    ok = false;
                    break;
                }
                match[j] = minun;
                used.insert(minun);
                for (auto it = skills[minun].begin(); it != skills[minun].end(); it++) {
                    maxskill[it->first] = max(maxskill[it->first], it->second);
                }
            }

            if (!ok) continue;

            any = true;
//            cerr << projects[i].score << " " << projects[i].dead << " " << projects[i].len << endl;

            forn(j, (int)match.size()) {
                busy[match[j]] = t + projects[i].len;
                if (skills[match[j]][projects[i].req[j].first] <= projects[i].req[j].second) {
                    skills[match[j]][projects[i].req[j].first]++;
                }
            }
            sol.push_back(mp(projects[i].name, match));

            projects.erase(projects.begin() + i);
            break;
        }

        bool allFree = true;
        for (const auto& x : skills) {
            if (!(busy[x.first] < t)) {
                allFree = false;
                break;
            }
        }
        if (!any)
            t++;
        if (allFree || !anyCost) break;
    }

    cout << sol.size() << endl;
    forn(i, (int)sol.size()) {
        cout << sol[i].first << endl;
        forn(j, (int)sol[i].second.size()) {
            cout << sol[i].second[j] << ' ';
        }
        cout << endl;
    }

    return 0;
}





//#include <bits/stdc++.h>
//#include <iostream>
//#include <fstream>
//
//using namespace std;
//
//struct Person {
//    string name;
//    unordered_map<string, int> skills;
//
//    Person(const string &name, const unordered_map<string, int> &skills) : name(name), skills(skills) {}
//};
//
//struct Task {
//    string name;
//    int duration;
//    int deadline;
//    int points;
//    unordered_map<string, int> skills;
//
//    Task(const string &name, int duration, int deadline, int points, const unordered_map<string, int> &skills) : name(
//            name), duration(duration), deadline(deadline), points(points), skills(skills) {}
//};
//
//
//int main() {
//    ifstream cin("input.txt");
//    ifstream cout("output.txt");
//
//    int c;
//    int p;
//    cin >> c >> p;
//    vector<Person> persons;
//    for (int i = 0; i < c; ++i) {
//        string name;
//        int cnt;
//        cin >> name >> cnt;
//        unordered_map<string, int> skills;
//        for (int j = 0; j < cnt; ++j) {
//            string nm;
//            int vl;
//            cin >> nm >> vl;
//            skills[nm] = vl;
//        }
//        persons.emplace_back(name, skills);
//    }
//
//    vector<Task> tasks;
//    for (int i = 0; i < c; ++i) {
//        string name;
//        int d;
//        int s;
//        int b;
//        int cnt;
//        cin >> name >> cnt;
//        unordered_map<string, int> skills;
//        for (int j = 0; j < cnt; ++j) {
//            string nm;
//            int vl;
//            cin >> nm >> vl;
//            skills[nm] = vl;
//        }
//        tasks.emplace_back(name, d, b, s, skills);
//    }
//    sort(tasks.begin(), tasks.end(), [&](const Task& lhs, const Task& rhs) {
//        return (lhs.deadline - lhs.duration) - (rhs.deadline - rhs.duration);
//    });
//
//    vector<Task> free = tasks;
//    vector<Task> runned;
//    int max_time = 3000;
//    for (int i = 0; i < max_time; ++i) {
//
//    }
//
//    return 0;
//}
