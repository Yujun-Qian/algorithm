class Solution {
private:
    int nVertex;
    int nEdge;
    map<string, int> airport2int;
    map<int, string> int2Airport;
   
    vector<vector<int>> graph;
    vector<int> outDegree;
    vector<int> inDegree;

    int buildGraphHelper(string airport) {
        int v;
        vector<int> placeholder;
        if (airport2int.find(airport) == airport2int.end()) {
            v = nVertex;
            airport2int[airport] = nVertex;
            int2Airport[nVertex++] = airport;
            graph.push_back(placeholder);
            outDegree.push_back(0);
            inDegree.push_back(0);
        } else {
            v = airport2int[airport];
        }
        return v;
    }

    void buildGraph(vector<vector<string>>& tickets) {
        nVertex = 0;
        nEdge = 0;
        for (auto ticket: tickets) {
            string from = ticket[0];
            string to = ticket[1];
           
            int v = buildGraphHelper(from);
            int w = buildGraphHelper(to);

            outDegree[v]++;
            inDegree[w]++;
            graph[v].push_back(w);
            nEdge++;
        }
    }
   
    void buildItinerary(int v, vector<string>& result) {
        vector<int>& vertices = graph[v];
           
        for (auto vertex : vertices) {
            if (vertex == -1) 
                continue;
            auto iter = find(vertices.begin(), vertices.end(), vertex);
            *iter = -1;
            buildItinerary(vertex, result);
        }
            
        result.push_back(int2Airport[v]);
    }
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        vector<string> result;

        buildGraph(tickets);
        int v = airport2int["JFK"];
       
        buildItinerary(v, result);
        std::reverse(result.begin(), result.end());
        return result;
    }
   
    virtual ~Solution() {
    }
};
