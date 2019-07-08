#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <ctime>

using namespace std;

struct city {
    string name;
    double lat, lng;
    int population = 0;
    vector<city*> connected_to;
    vector<double> length;
};

bool st(city A, city B) {
    return (A.population > B.population);
}

vector<city> all, working_with;
vector<city> output;
vector<pair<city, city>> kruskalList;
vector<pair<double, double>> visited;

vector<city> graph;

double distance(city a, city b) { //Haversine formula
  int R = 6371;
  double f1 = a.lat*3.14159265359/180;
  double f2 = b.lat*3.14159265359/180;
  double df = (b.lat - a.lat)*3.14159265359/180;
  double dl = (b.lng - a.lng)*3.14159265359/180;
  double one = sin(df/2)*sin(df/2) + cos(f1)*cos(f2)*sin(dl/2)*sin(dl/2);
  double two = 2 * atan2(sqrt(one), sqrt(1-one));
  return R * two;
}


void filter() {
    ifstream file("CitiesRaw.csv");
    ofstream file1("Cities1.csv");
    string s;
    int counter = 1;
    while (getline(file, s)) {
        stringstream ss(s);
        string word;
        while (getline(ss, word, ',')) {
            word.erase(word.size() - 1);
            word.erase(0, 1);
            if (counter == 1 || counter == 7 || counter == 8 || counter == 9) {
                if (counter != 1)
                    file1 << ',';
                if (counter == 9 && word[word.size()-2] == '.')
                    word.erase(word.size() - 2,2);
                file1 << word;
            }
            if (counter == 9) {
                file1 << '\n';
                counter = 1;
                break;
            }
            ++counter;
        }
    }
    file.close();
    file1.close();
}

void filter2_fill(int set) {
    ifstream file("Cities1.csv");
    ofstream file1("Cities.csv");
    string s;
    while (getline(file, s)) {
        stringstream ss(s);
        string word;
        int c = -1;
        city oldTownRoad;
        while (getline(ss, word, ',')) {
            c++;
            if (c % 4 == 0)
                oldTownRoad.name = word;
            else if (c % 4 == 1)
                oldTownRoad.lat = stod(word);
            else if (c % 4 == 2)
                oldTownRoad.lng = stod(word);
            else
                oldTownRoad.population = stoi(word);
        }
        all.push_back(oldTownRoad);
    }
    cout << "starting sorting...\n";
    sort(all.begin(), all.end(), st);
    all.resize(set);
    working_with = all;
    int counter = 0;
    for (auto town : all) {
        counter++;
        if (counter > set)
            break;
        file1 << town.name << "," << town.lat << "," << town.lng << "," << town.population << '\n';
    }
}

void set_all_connections() {
    ofstream file("Connections.csv");
    for (int i = 0; i < all.size()-1; ++i) {
        if (i+1 >= all.size())
            break;
        for (int j = i+1; j < all.size(); ++j) {
            if (i == j)
                break;
            all[i].connected_to.push_back(&all[j]);
            all[j].connected_to.push_back(&all[i]);
            double len = distance(all[i], all[j]);
            all[i].length.push_back(len);
            all[j].length.push_back(len);
            file << all[i].lat << ',' << all[i].lng <<
                 ',' << all[j].lat << ',' << all[j].lng << ',' << len << '\n';
        }
    }
    file.close();
}


city findCity(double x, double y)
{
  city no;
  for (auto i : all)
    if (i.lat == x && i.lng == y)
      return i;
  return no;
}

double lenEdge(pair<city, city> p)
{
    return distance(p.first, p.second);
}

bool KrSort (pair<city, city> A, pair<city, city> B)
{
    return (lenEdge(A) < lenEdge(B));
}

void kruskalListFiller()
{
    vector<pair<city, city>> output;
    pair<city, city> p;
    for (int i = 0; i < all.size()-1; ++i) {
      for (int j = i+1; j < all.size(); ++j) {
        p.first = all[i]; p.second = all[j];
        output.push_back(p);
      }
    }
    kruskalList = output;
    cout << "sorting kruskalList...\n";
    sort(kruskalList.begin(), kruskalList.end(), KrSort);
}

size_t findT(city A)
{
    for (int i = 0; i < all.size(); ++i)
      if (A.lat == all[i].lat && A.lng == all[i].lng) 
        return i;
    return 0;
}

bool BFS(pair<double, double> c, pair<double, double> CameFrom, city cc) {
    for (int i = 0; i < visited.size(); ++i)
        if (c.first == visited[i].first && c.second == visited[i].second)
            return true;
    visited.push_back(c);
    for (auto i : cc.connected_to) {
        pair<double, double> temp1; temp1.first = i->lat; temp1.second = i->lng;
        if (temp1 != CameFrom && BFS(temp1, c, *i))
            return true;
    }
    return false;
}

bool isCyclic(city inp) {
    visited.clear();
    pair<double, double> a;
    a.first = inp.lat; a.second = inp.lng;
    if (BFS(a, a, inp)) return true;
    return false;
}

bool NotIn(vector<city> a, city c) {
  for (auto i : a)
    if (i.lat == c.lat && i.lng == c.lng)
      return false;
  return true;
}

void buildSpanTree()
{
    for (int i = 0; i < all.size(); ++i) {
      all[i].connected_to.clear();
      all[i].length.clear();
    }
    for (int i = 0; i < kruskalList.size(); ++i) {
      kruskalList[i].first.connected_to.clear();
      kruskalList[i].second.connected_to.clear();
    }
    for (auto edge : kruskalList)
    {   
        size_t it1 = findT(edge.first);
        size_t it2 = findT(edge.second);
        all[it1].connected_to.push_back(&all[it2]);
        all[it2].connected_to.push_back(&all[it1]);
        if(isCyclic(all[it1]))
        {
            all[it1].connected_to.erase(all[it1].connected_to.end() - 1);
            all[it2].connected_to.erase(all[it2].connected_to.end() - 1);
        }
    }
    vector<city> vis;
    ofstream file("ConnectionsGraph.csv");
    for (int i = 0; i < all.size(); ++i) {
      vis.push_back(all[i]);
      for (auto j : all[i].connected_to) {
        if (NotIn(vis, *j)) {
          pair<city, city> p; p.first = all[i]; p.second = *j;
          stringstream ss;
          file << all[i].lat << ',' << all[i].lng << ',' << j->lat << ',' <<
          j->lng << ',' << lenEdge(p) << '\n';
        }
      }
    }
}

void tableBuilder()
{
    ifstream fileR("Cities.csv");
    ofstream output("table.csv");
    output << ";";
    for(city Town : all)
    {
        output << Town.name << ";";
    }
    output << "\n";
    for(city Town : all)
    {
        output << Town.name << ";";
        for(city Town1 : all)
        {
            output << lenEdge({Town,Town1}) << ";";
        }
        output << "\n";
    }
}

int main() {
  int number;
  cout << "Input the number of cities: ";
  cin >> number;
  time_t a = clock();
  cout << "first filter...\n";
  filter();
  cout << "second filter start...\n";
  filter2_fill(number);
  cout << "connecting all...\n";
  set_all_connections();
  cout << "filling kruskalList...\n";
  kruskalListFiller();
  buildSpanTree();
  tableBuilder();
  
  cout << "\nexecution time: " << (double)(clock() - a)/CLOCKS_PER_SEC;
}