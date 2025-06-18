#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>
#include <windows.h>
#include <chrono>

#include "tinyxml2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using std::string;
using std::vector;
using std::map;
using std::tuple;
using std::pair;
using std::make_pair;
using std::make_tuple;
using std::cout;
using std::endl;
using std::cerr;
using std::cin;

typedef pair<double, double> Coord;
typedef tuple<Coord, Coord, string> EdgeItem;
typedef map<Coord, vector<pair<Coord, double>>> Graph;

// Функция haversine: вычисляет расстояние между двумя точками на поверхности Земли (в километрах)
double haversine(const Coord& coord1, const Coord& coord2) {
    double lon1 = coord1.first, lat1 = coord1.second;
    double lon2 = coord2.first, lat2 = coord2.second;
    double R = 6371.0;
    double phi1 = lat1 * M_PI / 180.0;
    double phi2 = lat2 * M_PI / 180.0;
    double dphi = (lat2 - lat1) * M_PI / 180.0;
    double dlambda = (lon2 - lon1) * M_PI / 180.0;
    double a = std::sin(dphi / 2.0) * std::sin(dphi / 2.0) + std::cos(phi1) * std::cos(phi2) * std::sin(dlambda / 2.0) * std::sin(dlambda / 2.0);
    return 2 * R * std::atan2(std::sqrt(a), std::sqrt(1 - a));
}

// реализация алгоритма Дейкстры
tuple<vector<Coord>, double, vector<string>> dijkstra(const Graph& graph, Coord start, Coord end) {
    typedef pair<double, Coord> q_item;
    std::priority_queue<q_item, vector<q_item>, std::greater<q_item>> pq;

    map<Coord, double> dist;
    map<Coord, Coord> prev;

    // Инициализируем расстояния для всех вершин, которые есть в графе
    for (auto const& [node, neighbors] : graph) {
        dist[node] = std::numeric_limits<double>::infinity();
        // Убедимся, что и соседи тоже есть в карте расстояний
        for (const auto& edge : neighbors) {
            dist[edge.first] = std::numeric_limits<double>::infinity();
        }
    }

    dist[start] = 0;
    pq.push({ 0.0, start });

    while (!pq.empty()) {
        Coord u = pq.top().second;
        double d = pq.top().first;
        pq.pop();

        if (d > dist[u]) {
            continue;
        }
        if (u == end) {
            break;
        }

        if (graph.count(u)) {
            for (const auto& edge : graph.at(u)) {
                Coord v = edge.first;
                double weight = edge.second;
                if (dist.count(v) && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push({ dist[v], v });
                }
            }
        }
    }

    vector<Coord> path;
    if (dist.find(end) == dist.end() || dist[end] == std::numeric_limits<double>::infinity()) {
        return make_tuple(path, -1.0, vector<string>());
    }

    for (Coord at = end; ; at = prev[at]) {
        path.push_back(at);
        if (at == start) break;
    }
    std::reverse(path.begin(), path.end());

    return make_tuple(path, dist[end], vector<string>());
}


Graph build_graph(const vector<EdgeItem>& edges) {
    Graph graph;
    for (const auto& edge : edges) {
        Coord start, end;
        string street;
        std::tie(start, end, street) = edge;
        double dist = haversine(start, end);
        graph[start].push_back(make_pair(end, dist));
        graph[end].push_back(make_pair(start, dist));
    }
    return graph;
}

tuple<map<string, Coord>, vector<EdgeItem>> read_graphml(const string& file_path) {
    map<string, Coord> nodes;
    vector<EdgeItem> edges;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        cerr << "Oshibka: ne udalos prochitat fail grafa: " << file_path << endl;
        return make_tuple(nodes, edges);
    }

    tinyxml2::XMLElement* graphml = doc.FirstChildElement("graphml");
    tinyxml2::XMLElement* graph = graphml ? graphml->FirstChildElement("graph") : nullptr;

    if (!graph) return make_tuple(nodes, edges);

    for (tinyxml2::XMLElement* node = graph->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) {
        const char* id = node->Attribute("id");
        if (!id) continue;

        double x = 0, y = 0;
        for (tinyxml2::XMLElement* data = node->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (!key) continue;
            if (strcmp(key, "d5") == 0) x = atof(data->GetText());
            else if (strcmp(key, "d4") == 0) y = atof(data->GetText());
        }
        nodes[id] = make_pair(x, y);
    }

    for (tinyxml2::XMLElement* edge = graph->FirstChildElement("edge"); edge; edge = edge->NextSiblingElement("edge")) {
        const char* source_id = edge->Attribute("source");
        const char* target_id = edge->Attribute("target");
        if (!source_id || !target_id) continue;

        auto source_it = nodes.find(source_id);
        auto target_it = nodes.find(target_id);
        if (source_it == nodes.end() || target_it == nodes.end()) continue;

        string street_name;
        for (tinyxml2::XMLElement* data = edge->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (!key) continue;
            // ключ d14 для Варшавы, в примере был d18
            if (strcmp(key, "d14") == 0 && data->GetText()) {
                street_name = data->GetText();
            }
        }
        edges.emplace_back(source_it->second, target_it->second, street_name);
    }
    return make_tuple(nodes, edges);
}

pair<Coord, string> find_start_node_by_street_name(const vector<EdgeItem>& edges, const string& street_name) {
    for (const auto& edge : edges) {
        if (std::get<2>(edge) == street_name) {
            return { std::get<0>(edge), std::get<2>(edge) };
        }
    }
    return { {0,0}, "" };
}


void visualize_path_with_network(const map<string, Coord>& nodes, const vector<EdgeItem>& edges, const vector<Coord>& path) {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Shortest Path");

    double minX = std::numeric_limits<double>::max(), maxX = -std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max(), maxY = -std::numeric_limits<double>::max();

    for (auto const& [id, coords] : nodes) {
        minX = std::min(minX, coords.first);
        maxX = std::max(maxX, coords.first);
        minY = std::min(minY, coords.second);
        maxY = std::max(maxY, coords.second);
    }

    auto transform = [&](const Coord& c) {
        double scaleX = (760.0) / (maxX - minX);
        double scaleY = (760.0) / (maxY - minY);
        float x = static_cast<float>((c.first - minX) * scaleX + 20.0);
        float y = static_cast<float>(800 - ((c.second - minY) * scaleY + 20.0));
        return sf::Vector2f(x, y);
        };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);

        for (const auto& edge : edges) {
            sf::Vertex line[] = {
                sf::Vertex(transform(std::get<0>(edge)), sf::Color(220, 220, 220)),
                sf::Vertex(transform(std::get<1>(edge)), sf::Color(220, 220, 220))
            };
            window.draw(line, 2, sf::Lines);
        }

        if (path.size() > 1) {
            for (size_t i = 0; i < path.size() - 1; i++) {
                sf::Vertex line[] = {
                    sf::Vertex(transform(path[i]), sf::Color::Red),
                    sf::Vertex(transform(path[i + 1]), sf::Color::Red)
                };
                window.draw(line, 2, sf::Lines);
            }
        }
        window.display();
    }
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);

    map<string, Coord> nodes;
    vector<EdgeItem> edges;
    std::tie(nodes, edges) = read_graphml("warsaw_road_network.graphml");

    if (edges.empty()) {
        cout << "Oshibka: ne udalos prochitat fail grafa. Ubedites, chto warsaw_road_network.graphml nahoditsya v papke proekta." << endl;
        system("pause");
        return 1;
    }

    cout << "Graph zagruzhen. Vershin: " << nodes.size() << ", Reber: " << edges.size() << endl;

    vector<string> unique_streets;
    map<string, bool> streets_map;
    for (const auto& edge : edges) {
        const string& name = std::get<2>(edge);
        if (!name.empty() && streets_map.find(name) == streets_map.end()) {
            unique_streets.push_back(name);
            streets_map[name] = true;
        }
    }
    std::sort(unique_streets.begin(), unique_streets.end());

    cout << "\n--- Spisok dostupnyh ulits ---" << endl;
    for (size_t i = 0; i < unique_streets.size(); ++i) {
        cout << i + 1 << ". " << unique_streets[i] << endl;
    }
    cout << "--------------------------------\n" << endl;

    int start_idx, end_idx;
    cout << "Vvedite nomer nachalnoy ulitsy: ";
    cin >> start_idx;
    cout << "Vvedite nomer konechnoy ulitsy: ";
    cin >> end_idx;

    if (start_idx <= 0 || start_idx > unique_streets.size() || end_idx <= 0 || end_idx > unique_streets.size()) {
        cerr << "Oshibka: nevernyy nomer ulitsy." << endl;
        system("pause");
        return 1;
    }

    string start_street = unique_streets[start_idx - 1];
    string end_street = unique_streets[end_idx - 1];

    auto start_data = find_start_node_by_street_name(edges, start_street);
    auto end_data = find_start_node_by_street_name(edges, end_street);

    if (start_data.second.empty() || end_data.second.empty()) {
        cout << "Ne udalos nayti odnu iz ulits." << endl;
    }
    else {
        cout << "\nStart: " << start_data.second << endl;
        cout << "Konets: " << end_data.second << endl;

        Graph graph = build_graph(edges);

        auto time_start = std::chrono::high_resolution_clock::now();
        auto [path, distance, street_names] = dijkstra(graph, start_data.first, end_data.first);
        auto time_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed_time = time_end - time_start;

        if (distance < 0) {
            cout << "\nPut mezhdu tochkami ne nayden." << endl;
        }
        else {
            cout << "\n----------------------------------" << endl;
            cout << "Rezultaty poiska:" << endl;
            cout << "Dlina puti: " << distance << " km." << endl;
            cout << "Vremya raboty algoritma: " << elapsed_time.count() << " sekund." << endl;
            cout << "----------------------------------" << endl;
            visualize_path_with_network(nodes, edges, path);
        }
    }

    cout << "\nProgramma zavershena." << endl;
    system("pause");
    return 0;
}