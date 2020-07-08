# City-Connector
The map is available at the link:
https://city-connector.herokuapp.com

Demonstration:
![](https://github.com/DKarz/readme-media/blob/master/city-conn1.gif?raw=true)

![](https://github.com/DKarz/readme-media/blob/master/city-conn22.gif?raw=true)

![](https://github.com/DKarz/readme-media/blob/master/city-conn3.gif?raw=true)


### Internship goals: 
consolidation, expansion and deepening of the theoretical knowledge and acquirement of initial practical skills in solving specific problems 
### Tasks:	
a.	consolidation and deepening of the theoretical knowledge on the disciplines passed at the university;
b.	acquirement of information competence for the purpose of successful work in professional activities;
c.	obtaining skills of both independent and team work.


For a given country, it is necessary to connect cities by roads so that you can get from any city to any other. Cities are given by their geographical coordinates, the roads are drawn directly and the length of each is the Euclidean distance between them. The target spanning tree should be visualized and obtained distances should be compared with existing.

My team and I were working on US City Connector. The main aim of the project is to represent the connections between some cities of the United States in the form of a spanning tree. Mainly, we worked with C++ language, with some help from python and its libraries (folium). In this report, I will describe all the stages of the project, give a brief description of the work made by my teammates, and detailed (as possible) explanation of my activity. 
First of all, data about US cities were obtained from the internet and saved as csv file by me. Apart from important information such as the name of a city, its geographical coordinates and the number of residents, data set provides us with the name of the state, the area it covers, etc. that we considered as unnecessary. 
  
Therefore, I created auxiliary function filter() which cleans the file’s copy and saves a new file (Cities1.csv) consisting of names of towns, the coordinates, and population.
```cpp
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
                file1 << endl;
                counter = 1;
                break;
            }
            ++counter;
        }
    }
    file.close();
    file1.close();
}
```
Since it is nearly impossible to consider all the cities of the United States as there are roughly 30000 of them, we decided to choose 150 of them. My colleagues suggested choosing them randomly which will increase the chance to cover more states. However, I disagreed with that as the list contains many unknown towns (I have not even heard about their existence). So, I offered to take the first 150 cities from the list sorted by population. As it was my idea, I had to realize it. To do that I defined another auxiliary function filter2_fill().
```cpp
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
        file1 << town.name << "," << town.lat << "," << town.lng << "," << town.population << endl;
    }
}
```

At this stage we were almost ready to proceed to the graph. The class city created by my comrades, I asked to replace by struct city.
```cpp
struct city {
    string name;
    double lat, lng;
    int population = 0;
    vector<city*> connected_to;
};
```
Two notable benefits made me do this; firstly, it helps to avoid writing some necessary parts of each class such as constructor, destructor, etc., secondly, struct is more instinctively comprehensible. Moreover, file (Connections.csv) was created to store the edges with the following interface – each line consists of first, second coordinates of first and second city, and the distance between them. To construct the spanning tree, we chose the Kruskal algorithm, while my teammates worked on BFS and function which checks the cyclicity of the graph, I provided them with the function kruskalListFiller()which returns the list of all edges sorted by the length.
```cpp
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
```
There I made some additional functions – findCity(double x, double y) and lenEdge(pair<city, city> p) ,which return city by its coordinates and distance between two cities respectively.
 ```cpp
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
```

Description of the results
All auxiliary functions were used in creating csv file called ConnectionsGraph.csv containing all the ages of the spanning tree. The file was used in map building. There we used python and its library folium which gives an opportunity to work with maps.
 
 
 
Clicking on the icon of a town shows the number of people living there.
Finally, I decided to make a table with all the 150 towns we considered and the distance between them. 
 
 
I manage to do it automatically by creating tableBuilder() function, which saves the distances in csv file. One of the problems was to calculate the distance in kilometers, not in degrees. Function distance() written by the mates helps to tackle this issue.

Conclusion
It was really interesting to apply the knowledge acquired during a long year in the university in a real problem. Despite the fact that the greatest part of the project was made with the help of C++ language, I was excited to use python in the main part – constructing the map. At the beginning the diversity of different python libraries confused us, however, we managed to choose an appropriate one. The task made us learn about folium and how to use this library. One of the reasons we chose City-Connector is that we are able to “touch” the final result. It is not an indifferent program which output is a block of text. The result is an interactive map that enables users to scale, look at the map from different angles, and learn about the population of a town.

