// leetcode #332
class Solution {
private:
    int nVertex;
    int nEdge;
    map<string, int> airport2int;
    map<int, string> int2Airport;
   
    vector<vector<int>> graph;
    vector<int> outDegree;
    vector<int> inDegree;
   
    // UF method
    int root(int v, int* vertices) {
        while(vertices[v] != v) {
            vertices[v] = vertices[vertices[v]];
            v = vertices[v];
        }
       
        return v;
    }

    // UF method
    void connect(int i, int j, int* sizes, int* vertices) {
        int rootI = root(i, vertices);
        int rootJ = root(j, vertices);
        if (rootI == rootJ)
            return;
       
        if (sizes[rootI] < sizes[rootJ]) {
            vertices[rootI] = rootJ;
            sizes[rootJ] += sizes[rootI];
        } else {
            vertices[rootJ] = rootI;
            sizes[rootI] += sizes[rootJ];
        }
    }

    // count the number of components if we remove the edge
    int nComponents(int v) {
        int ret = 0;
        int* vertices = new int[nVertex];
        int* sizes = new int[nVertex];
       
        for (int i = 0; i < nVertex; i++) {
            if (outDegree[i] + inDegree[i] > 0 || i == v) {
                vertices[i] = i;
                sizes[i] = 1;
            } else {
                vertices[i] = -1;
                sizes[i] = 0;
            }
        }
       
        for (int i = 0; i < nVertex; i++) {
            for (int j: graph[i]) {
                //cout << "connect:  " << i << "," << j << endl;
                if (j != -1)
                    connect(i, j, sizes, vertices);
            }
        }
   
   
        for (int i = 0; i < nVertex; i++) {
            //cout << inDegree[i] << endl;
            if (vertices[i] == i)
                ret++;
        }
   
        //cout << "ret is:  " << ret << endl;

        delete[] vertices;
        delete[] sizes;
        return ret;
    }

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
        while(nEdge > 0) {
            vector<int>& vertices = graph[v];
            map<string, int> airports;
            for (int vertex : vertices) {
                if (vertex != -1)
                    airports[int2Airport[vertex]] = vertex;
            }
           
            // choose the next vertex
            for (auto iter : airports) {
                inDegree[iter.second]--;
                outDegree[v]--;
                //cout << outDegree[v] << endl;
                auto itr = std::find(graph[v].begin(), graph[v].end(), iter.second);
                if (itr != graph[v].end())
                    *itr = -1;
                if (nComponents(iter.second) <= 1) {
                    v = iter.second;
                    result.push_back(iter.first);
                    break;
                }
                inDegree[iter.second]++;
                outDegree[v]++;
                if (itr != graph[v].end())
                    *itr = iter.second;
            }
           
            nEdge--;
        }
    }
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        vector<string> result;

        buildGraph(tickets);
        int v = airport2int["JFK"];
        result.push_back("JFK");
       
        buildItinerary(v, result);
        return result;
    }
   
    virtual ~Solution() {
    }
};
