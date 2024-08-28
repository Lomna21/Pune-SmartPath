#include<bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <limits>
#include <algorithm>
#include <functional>
#include <cmath>
#include <cctype>
#include <map>
using namespace std;

class Graph_M
{
    public:
        class Vertex {
            public:
                unordered_map<string, int> nbrs;
        };

        static unordered_map<string, Vertex> vtces;

        Graph_M() {
            // Constructor
        }

        int numVertex() {
            return vtces.size();
        }

        bool containsVertex(const string& vname) {
            return vtces.find(vname) != vtces.end();
        }

        void addVertex(const string& vname) {
            Vertex vtx;
            vtces[vname] = vtx;
        }

        void removeVertex(const string& vname) {

            Vertex& vtx = vtces[vname];
            vector<string> keys;
            
            for (const auto& pair : vtx.nbrs) {
                keys.push_back(pair.first);
            }

            for (const string& key : keys) {
                Vertex& nbrVtx = vtces[key];
                nbrVtx.nbrs.erase(vname);
            }
            vtces.erase(vname);
        }

        int numEdges() {

            int count = 0;
            for (const auto& pair : vtces) {
                count += pair.second.nbrs.size();
            }
            return count / 2;
        }

        bool containsEdge(const string& vname1, const string& vname2) {

            if (vtces.find(vname1) == vtces.end() || vtces.find(vname2) == vtces.end()) {
                return false;
            }

            Vertex& vtx1 = vtces[vname1];
            Vertex& vtx2 = vtces[vname2];
            
            if (vtx1.nbrs.find(vname2) == vtx1.nbrs.end()) {
                return false;
            }
            return true;
        }

        void addEdge(const string& vname1, const string& vname2, int value) 
        {
            if (vtces.find(vname1) == vtces.end() || vtces.find(vname2) == vtces.end()) {
                return;
            }

            Vertex& vtx1 = vtces[vname1];
            Vertex& vtx2 = vtces[vname2];

            if (vtx1.nbrs.find(vname2) != vtx1.nbrs.end()) {
                return;
            }

            vtx1.nbrs[vname2] = value;
            vtx2.nbrs[vname1] = value;
        }

        void removeEdge(const string& vname1, const string& vname2) 
        {
            if (vtces.find(vname1) == vtces.end() || vtces.find(vname2) == vtces.end()) {
                return;
            }

            Vertex& vtx1 = vtces[vname1];
            Vertex& vtx2 = vtces[vname2];
            
            if (vtx1.nbrs.find(vname2) == vtx1.nbrs.end()) {
                return;
            }

            vtx1.nbrs.erase(vname2);
            vtx2.nbrs.erase(vname1);
        }

        void display_Map() 
        {
            cout << "\t Delhi Metro Map" << endl;
            cout << "\t------------------" << endl;
            cout << "----------------------------------------------------\n" << endl;

            for (const auto& pair : vtces) 
            {
                string str = pair.first + " =>\n";
                Vertex vtx = pair.second;
                
                for (const auto& nbrPair : vtx.nbrs) 
                {
                    str += "\t" + nbrPair.first + "\t";
                    if (nbrPair.first.length() < 16) str += "\t";
                    if (nbrPair.first.length() < 8) str += "\t";
                    str += to_string(nbrPair.second) + "\n";
                }
                cout << str;
            }

            cout << "\t------------------" << endl;
            cout << "---------------------------------------------------\n" << endl;
        }

        void display_Stations() {
            cout << "\n***********************************************************************\n" << endl;
            int i = 1;
            for (const auto& pair : vtces) 
            {
                cout << i << ". " << pair.first << endl;
                ++i;
            }
            cout << "\n***********************************************************************\n" << endl;
        }
            
        bool hasPath(const string& vname1, const string& vname2, unordered_map<string, bool>& processed) {
            if (containsEdge(vname1, vname2)) {
                return true;
            }

            processed[vname1] = true;

            Vertex& vtx = vtces[vname1];
            for (const auto& nbrPair : vtx.nbrs) 
            {
                const string& nbr = nbrPair.first;
                if (processed.find(nbr) == processed.end())
                {
                    if (hasPath(nbr, vname2, processed))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        struct DijkstraPair 
        {
            string vname;
            string psf;
            int cost;

            bool operator>(const DijkstraPair& other) const 
            {
                return cost > other.cost;
            }
        };

        int dijkstra(const string& src, const string& des, bool nan) 
        {
            int val = 0;
            vector<string> ans;
            unordered_map<string, DijkstraPair> map;

            auto cmp = [](DijkstraPair left, DijkstraPair right) { return left.cost > right.cost; };
            priority_queue<DijkstraPair, vector<DijkstraPair>, decltype(cmp)> heap(cmp);

            for (const auto& pair : vtces) 
            {
                DijkstraPair np;
                np.vname = pair.first;
                np.psf = "";
                np.cost = numeric_limits<int>::max();

                if (pair.first == src) 
                {
                    np.cost = 0;
                    np.psf = pair.first;
                }

                heap.push(np);
                map[pair.first] = np;
            }

            while (!heap.empty()) 
            {
                DijkstraPair rp = heap.top();
                heap.pop();

                if (rp.vname == des)
                {
                    val = rp.cost;
                    break;
                }

                map.erase(rp.vname);
                ans.push_back(rp.vname);

                Vertex& v = vtces[rp.vname];
                for (const auto& nbrPair : v.nbrs) 
                {
                    const string& nbr = nbrPair.first;
                    if (map.find(nbr) != map.end()) 
                    {
                        int oc = map[nbr].cost;
                        Vertex& k = vtces[rp.vname];
                        int nc = (nan) ? rp.cost + 120 + 40 * k.nbrs[nbr] : rp.cost + k.nbrs[nbr];

                        if (nc < oc) 
                        {
                            DijkstraPair& gp = map[nbr];
                            gp.psf = rp.psf + nbr;
                            gp.cost = nc;

                            heap.push(gp);
                        }
                    }
                }
            }
            return val;
        }
        class Pair 
        {
        public:
            string vname;
            string psf;
            int min_dis;
            int min_time;
        };

        string Get_Minimum_Distance(const string& src, const string& dst) 
        {
            int min = numeric_limits<int>::max();
            string ans = "";
            unordered_map<string, bool> processed;
            deque<Pair> stack;

            Pair sp;
            sp.vname = src;
            sp.psf = src + "  ";
            sp.min_dis = 0;
            sp.min_time = 0;

            stack.push_front(sp);

            while (!stack.empty()) 
            {
                Pair rp = stack.front();
                stack.pop_front();

                if (processed.find(rp.vname) != processed.end()) 
                {
                    continue;
                }

                processed[rp.vname] = true;

                if (rp.vname == dst) 
                {
                    int temp = rp.min_dis;
                    if (temp < min) 
                    {
                        ans = rp.psf;
                        min = temp;
                    }
                    continue;
                }

                Vertex& rpvtx = vtces[rp.vname];
                for (const auto& nbrPair : rpvtx.nbrs) 
                {
                    const string& nbr = nbrPair.first;
                    if (processed.find(nbr) == processed.end()) 
                    {
                        Pair np;
                        np.vname = nbr;
                        np.psf = rp.psf + nbr + "  ";
                        np.min_dis = rp.min_dis + rpvtx.nbrs[nbr];
                        stack.push_front(np);
                    }
                }
            }
            ans += to_string(min);
            return ans;
        }

        string Get_Minimum_Time(const string& src, const string& dst) 
        {
            int min = numeric_limits<int>::max();
            string ans = "";
            unordered_map<string, bool> processed;
            deque<Pair> stack;

            Pair sp;
            sp.vname = src;
            sp.psf = src + "  ";
            sp.min_dis = 0;
            sp.min_time = 0;

            stack.push_front(sp);

            while (!stack.empty()) 
            {
                Pair rp = stack.front();
                stack.pop_front();

                if (processed.find(rp.vname) != processed.end()) 
                {
                    continue;
                }

                processed[rp.vname] = true;

                if (rp.vname == dst) 
                {
                    int temp = rp.min_time;
                    if (temp < min) 
                    {
                        ans = rp.psf;
                        min = temp;
                    }
                    continue;
                }

                Vertex& rpvtx = vtces[rp.vname];
                for (const auto& nbrPair : rpvtx.nbrs) 
                {
                    const string& nbr = nbrPair.first;
                    if (processed.find(nbr) == processed.end()) 
                    {
                        Pair np;
                        np.vname = nbr;
                        np.psf = rp.psf + nbr + "  ";
                        np.min_time = rp.min_time + 120 + 40 * rpvtx.nbrs[nbr];
                        stack.push_front(np);
                    }
                }
            }
            double minutes = ceil(static_cast<double>(min) / 60);
            ans += to_string(minutes);
            return ans;
        }
        vector<string> get_Interchanges(const string& str) 
        {
            vector<string> arr;
            stringstream ss(str);
            string token;
            vector<string> res;

            while (getline(ss, token, ' ')) {
                if (!token.empty()) {
                    res.push_back(token);
                }
            }

            arr.push_back(res[0]);
            int count = 0;

            for (size_t i = 1; i < res.size() - 1; i++) 
            {
                size_t index = res[i].find('~');
                string s = res[i].substr(index + 1);

                if (s.length() == 2) 
                {
                    string prev = res[i - 1].substr(res[i - 1].find('~') + 1);
                    string next = res[i + 1].substr(res[i + 1].find('~') + 1);

                    if (prev == next) 
                    {
                        arr.push_back(res[i]);
                    } 
                    else 
                    {
                        arr.push_back(res[i] + " ==> " + res[i + 1]);
                        i++;
                        count++;
                    }
                } 
                else 
                {
                    arr.push_back(res[i]);
                }
            }

            arr.push_back(to_string(count));
            arr.push_back(res[res.size() - 1]);
            return arr;
        }

        static void Create_Metro_Map(Graph_M& g) 
        {
            g.addVertex("Noida Sector 62~B");
            g.addVertex("Botanical Garden~B");
            g.addVertex("Yamuna Bank~B");
            g.addVertex("Rajiv Chowk~BY");
            g.addVertex("Vaishali~B");
            g.addVertex("Moti Nagar~B");
            g.addVertex("Janak Puri West~BO");
            g.addVertex("Dwarka Sector 21~B");
            g.addVertex("Huda City Center~Y");
            g.addVertex("Saket~Y");
            g.addVertex("Vishwavidyalaya~Y");
            g.addVertex("Chandni Chowk~Y");
            g.addVertex("New Delhi~YO");
            g.addVertex("AIIMS~Y");
            g.addVertex("Shivaji Stadium~O");
            g.addVertex("DDS Campus~O");
            g.addVertex("IGI Airport~O");
            g.addVertex("Rajouri Garden~BP");
            g.addVertex("Netaji Subhash Place~PR");
            g.addVertex("Punjabi Bagh West~P");

            g.addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
            g.addEdge("Botanical Garden~B", "Yamuna Bank~B", 10);
            g.addEdge("Yamuna Bank~B", "Vaishali~B", 8);
            g.addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
            g.addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
            g.addEdge("Moti Nagar~B", "Janak Puri West~BO", 7);
            g.addEdge("Janak Puri West~BO", "Dwarka Sector 21~B", 6);
            g.addEdge("Huda City Center~Y", "Saket~Y", 15);
            g.addEdge("Saket~Y", "AIIMS~Y", 6);
            g.addEdge("AIIMS~Y", "Rajiv Chowk~BY", 7);
            g.addEdge("Rajiv Chowk~BY", "New Delhi~YO", 1);
            g.addEdge("New Delhi~YO", "Chandni Chowk~Y", 2);
            g.addEdge("Chandni Chowk~Y", "Vishwavidyalaya~Y", 5);
            g.addEdge("New Delhi~YO", "Shivaji Stadium~O", 2);
            g.addEdge("Shivaji Stadium~O", "DDS Campus~O", 7);
            g.addEdge("DDS Campus~O", "IGI Airport~O", 8);
            g.addEdge("Moti Nagar~B", "Rajouri Garden~BP", 2);
            g.addEdge("Punjabi Bagh West~P", "Rajouri Garden~BP", 2);
            g.addEdge("Punjabi Bagh West~P", "Netaji Subhash Place~PR", 3);
        }

        static vector<string> printCodelist(){
            cout << "List of station along with their codes:\n";
            vector<string> keys;
            for (const auto& kv : vtces) {
                keys.push_back(kv.first);
            }

            int i = 1, j = 0, m = 1;
            vector<string> codes(keys.size());
            for (const string& key : keys) 
            {
                istringstream stname(key);
                string temp;
                char c;

                codes[i - 1] = "";
                j = 0;
                while (stname >> temp) 
                {
                    c = temp[0];
                    while (isdigit(c)) 
                    {
                        codes[i - 1] += c;
                        j++;
                        c = temp[j];
                    }
                    if (!isdigit(c) && c < 123) 
                    {
                        codes[i - 1] += c;
                    }
                }
                if (codes[i - 1].length() < 2) 
                {
                    codes[i - 1] += toupper(temp[1]);
                }

                cout << i << ". " << key << "\t";
                if (key.length() < (22 - m)) cout << "\t";
                if (key.length() < (14 - m)) cout << "\t";
                if (key.length() < (6 - m)) cout << "\t";
                cout << codes[i - 1] << endl;
                i++;
                if (i == static_cast<int>(pow(10, m))) m++;
            }
            return codes;
        }
    
};
unordered_map<string, Graph_M::Vertex> Graph_M::vtces;

int main() {
    // Create a Graph_M object
    Graph_M g;
    
    // Call the static function to create the metro map
    Graph_M::Create_Metro_Map(g);

    // Welcome message
    cout << "\n\t\t\t****WELCOME TO THE METRO APP*****" << endl;

    // Main loop to keep the menu active
    while (true) {
        // Display the list of actions
        cout << "\t\t\t\t~~LIST OF ACTIONS~~\n\n";
        cout << "1. LIST ALL THE STATIONS IN THE MAP" << endl;
        cout << "2. SHOW THE METRO MAP" << endl;
        cout << "3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "7. EXIT THE MENU" << endl;
        cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 7) : ";

        // Variable to store user choice
        int choice = -1;
        cin >> choice;

        // Separator
        cout << "\n***********************************************************\n";

        // Exit the program if the choice is 7
        if (choice == 7) {
            exit(0);
        }

        // Switch case to handle different choices
        switch (choice) {
        case 1:
            // Display all stations
            g.display_Stations();
            break;

        case 2:
            // Display the metro map
            g.display_Map();
            break;

        case 3: {
            // Find the shortest distance
            vector<string> keys;
            for (const auto &pair : Graph_M::vtces) {
                keys.push_back(pair.first);
            }
            vector<string> codes = Graph_M::printCodelist();
            cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER CODE OF STATIONS\n3. TO ENTER NAME OF STATIONS\n";
            cout << "ENTER YOUR CHOICE: ";
            int ch;
            cin >> ch;
            cin.ignore();

            string st1, st2;
            // cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
            if (ch == 1) {

                int index1, index2;
                cout << "ENTER THE SOURCE STATION: ";
                cin >> index1;
                cout << "ENTER THE DESTINATION STATION: ";
                cin >> index2;
                st1 = keys[index1 - 1];
                st2 = keys[index2 - 1];
            
            } else if (ch == 2) {

                string a, b;
                cout << "ENTER THE SOURCE STATION: ";
                cin >> a;
                cout << "ENTER THE DESTINATION STATION: ";
                cin >> b;
                transform(a.begin(), a.end(), a.begin(), ::toupper);
                transform(b.begin(), b.end(), b.begin(), ::toupper);

                auto it1 = find(codes.begin(), codes.end(), a);
                auto it2 = find(codes.begin(), codes.end(), b);

                if (it1 != codes.end() && it2 != codes.end()) {
                    st1 = keys[distance(codes.begin(), it1)];
                    st2 = keys[distance(codes.begin(), it2)];
                } else {
                    cout << "Invalid station codes entered" << endl;
                }

            } else if (ch == 3) {

                cout << "ENTER THE SOURCE STATION: ";
                cin.ignore(); 
                getline(cin, st1); 
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, st2); 

            } else {

                cout << "Invalid choice" << endl;
                exit(0);
            }

            unordered_map<string, bool> processed;
            if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed)) {
                cout << "THE INPUTS ARE INVALID" << endl;
            } else {
                cout << "SHORTEST DISTANCE FROM " << st1 << " TO " << st2 << " IS " << g.dijkstra(st1, st2, false) << "KM\n";
            }
            break;
        }

        case 4: {
            
            // Find the shortest time
            string ss1, ss2;
            cout << "ENTER THE SOURCE STATION: ";
            cin.ignore(); 
            getline(cin, ss1); 
            cout << "ENTER THE DESTINATION STATION: ";
            getline(cin, ss2); 

            unordered_map<string, bool> processed;
            cout << "SHORTEST TIME FROM (" << ss1 << ") TO (" << ss2 << ") IS " << g.dijkstra(ss1, ss2, true) / 60 << " MINUTES\n\n";
            break;
        }

        case 5: {

            // Find the shortest path (distance-wise)
            string s1, s2;
            cout << "ENTER THE SOURCE STATION: ";
            cin.ignore(); // Clear the newline character from the buffer
            getline(cin, s1); // Read the full station name including spaces
            cout << "ENTER THE DESTINATION STATION: ";
            getline(cin, s2); 

            unordered_map<string, bool> processed;
            // if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed)) {
            //     cout << "THE INPUTS ARE INVALID" << endl;
            // } else {
                vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                int len = str.size();
                cout << "SOURCE STATION : " << s1 << endl;
                cout << "DESTINATION STATION : " << s2 << endl;
                cout << "DISTANCE : " << str[len - 1] << endl;
                cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                cout << "START  ==>  " << str[0] << " ==>  ";
                for (int i = 1; i < len - 3; i++) {
                    cout << str[i] << " ==>  ";
                }
                cout << str[len - 3] << "   ==>    END" << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            // }
            break;
        }

        case 6: {
            
            // Find the shortest path (time-wise)
            string ss1, ss2;
            cout << "ENTER THE SOURCE STATION: ";
            cin.ignore(); // Clear the newline character from the buffer
            getline(cin, ss1); // Read the full station name including spaces
            cout << "ENTER THE DESTINATION STATION: ";
            getline(cin, ss2); 

            unordered_map<string, bool> processed;
            if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed)) {
                cout << "THE INPUTS ARE INVALID" << endl;
            } else {
                vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(ss1, ss2));
                int len = str.size();
                cout << "SOURCE STATION : " << ss1 << endl;
                cout << "DESTINATION STATION : " << ss2 << endl;
                cout << "TIME : " << str[len - 1] << " MINUTES" << endl;
                cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                cout << "START  ==>  " << str[0] << " ==>  ";
                for (int i = 1; i < len - 3; i++) {
                    cout << str[i] << " ==>  ";
                }
                cout << str[len - 3] << "   ==>    END" << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            }
            break;
        }

        default:
            // Handle invalid option
            cout << "Please enter a valid option! " << endl;
            cout << "The options you can choose are from 1 to 7. " << endl;
        }
    }
    return 0;
}


