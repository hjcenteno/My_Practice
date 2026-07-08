/* 
  Henry Centeno
  netID: hcenteno 000-68-7923
  Date: 28 October 2025

  description: Utilizing a 2d matrix and graphs to determine the shortest path between 2 cities to determine two rates:
  a cheap rate determined by distance traveled, a more expensive rate determined by the time needed for travelling.
  The algorithm in dijkstra_rout() utilizes three colors to determine whether a city has been unvisited, visited once, or visited 
  twice. The source city will be set to black to say that the edge back to the source city should not be considered, and the weight
  from source to source will be set to 0. Each edge of the source city will then be added to a min-heap priority queue to ensure we 
  take the cheapest path, and a map where each value is a queue detailing the route of cities taken from the source to get there. 
  We will assume that all weights from the source to other cities are infinity stored in a vector for easy look up. Only push a 
  city in the queue if the current cost plus the weight to the city is less than its weight in this vector. The current cost is 
  defined as the sum of the weights towards the city from the source. We will add to the queue until the queue is empty, but we 
  will skip cities that are black, are the target city, or whose current cost is greater than the cost to the target. Each time we
  access a city in the queue, we  will pop the city from the queue. Utilizing gray can allow for mutliple routes to use a gray city 
  as long as the cost to get to this city is less than an establish cost. Once the cheapest route to get to the target city is 
  obtained, we can then output from the queue in the map the route taken from the source to the target city. 
  If the city is considered black, we will not establish a route to the city.
*/
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>
#include <math.h>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <iomanip>
#include <queue>
#include <limits>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::vector;
using std::strcmp;
using std::string;
using std::sort;
using std::map;
using std::ostream;
using std::ofstream;

#define DEG2RAD (M_PI/180.0)
#define RAD2DEG (180.0/M_PI)

const float earthradius = 3960.0;     // [miles]
const float distance_accuracy = 5.0;  // [miles]

const int city_minpop = 25000;
const int hub_minpop = 1000000;
const float hub_mindist = 130.0;      // [miles]

const float plane_speed = 640.0;      // [mph]
const float truck_speed = 65.0;       // [mph]

enum color_t { WHITE, GRAY, BLACK };
enum rmde_t { UNKNOWN, INFO, DIST, TIME};

class city {
  // Define the city class to store city name (string), latitude (float), longitude (float) and population (int). 
  // Make these private data members. Add public get_variable() and set_variable() functions for variables that need them. 
  // Provide an overloaded output operator to facilitate 
  // "fout << city" type code. Have this code convert latitudes and longitudes from radians back to degrees.
  private:
    string cityName;
    float  latitude; //(degrees)
    float  longitude; //(degrees)
    int    population;

  public:
    int w = 0; //width of characters of the maximum city length for formatting
    int index = 0; //incremental index to each city. use this for internal reference.
    color_t visited = WHITE; //white == unvisited; gray == visited once; black == visited twice

    void set_cityName(string name){ cityName = name; }
    void set_latitude(float lat){ latitude = lat; }
    void set_longitude(float lon){ longitude = lon; }
    void set_population(int pop){ population = pop; }
    
    void operator+=(city &c);

    string get_cityName(){ return cityName; }
    float  get_latitude(){ return latitude; }
    float  get_longitude(){ return longitude; }
    int    get_population(){ return population; }
};

//https://github.com/akalicki/matrix/blob/master/dist/matrix.cpp
/*
  I found this repository by akalicki for creating a matrix class in c++. I took inspiration in how to format and implement the matrix class 
  with liberties of my own. Most notably is that I am implementing the maxtrix class to be a matrix of <float, float> where the 
  rows are the to cities and columns are from cities and the data will be the distance/time between them

  example: using made up numbers, but the data in this matrix will be time in hours
  column row ...
  spacer     Knoxville    Oak Ridge    Atlanta
  Knoxville  0            .5           3.2

  Oak Ridge  .5           0            3.7

  Atlanta    3.2          3.7          0
*/ 
class matrix {
  // Define a 2D matrix class which you will use to store distance and time table information. 
  // Since the distance and thus time to travel is the same from A to B as for B to A, you should set this up as a symmetric matrix 
  // where only the diagonal and lower triangle of the full, square matrix is used. Instead of using an overloaded [] operator, 
  // define public get_value() and set_value() member functions since they give you full control over how the row and column indices are used. 
  // That said, you are free to implement this data structure any which way you like bu Have the main function declare matrix dist_table and 
  // time_table objects.
  
  public:
    matrix(int row); //create a matrix size of row^2 but most likely half the matrix will be half to account that A to B == B to A
    ~matrix(); //clear the vectors in data

    void set_value(int row, int col, float value); //pass in the from and add to city in its row
    float get_value(int row, int col);
    void print_table(const char *s); //debugger

    int size = 0;

  private:
    vector < vector <float> > data; //time or distance
};

matrix::matrix(int row){
  //initiate data to be size of row by row filled with 0.0
  //took inspiration from https://github.com/akalicki/matrix/blob/master/dist/matrix.cpp
  size = row;

  data.resize(row);
  for(int i = 0; i < (int)row; i++){
    data[i].resize(row, 0.0f);
  }
}

matrix::~matrix(){
  //clear all the vectors
  for(auto dataRow : data){
    dataRow.clear();
  }

  data.clear();
}

void matrix::set_value(int row, int col, float value){
  //handle symmetry of the table
  if(row == col){
    return; //the value is already set to 0.0
  }

  if(row > col){
    return; //already processed
  }

  data[row][col] = value;

}

float matrix::get_value(int row, int col){
  if(row > col){
    std::swap(row, col); //just look at what [col][row] is since we are populating row by row i.e (128, 0) to (0, 128)
  }

  return data[row][col];
}

void matrix::print_table(const char *s){
  //helper debugger function 
  ofstream fout;

  //open log file
  fout.open(s);
  if(!fout.is_open()){
    cerr << "unable to open " << s << endl;
    fout.close();
    return;
  }

  //write to log file
  for(auto &row : data){
      for(auto item : row){
          fout << std::setw(7) << std::right << std::setprecision(2) << std::fixed << item << " ";
      }
      fout << endl;
  }

  fout.close();
}

//taken and modified from lab1
void city::operator+=(city &c) {
  // update the city of the county if the city being read is greater than the current county city
  if(c.get_population() > population){
    cityName = c.get_cityName();
    latitude = c.get_latitude();
    longitude = c.get_longitude();
    population = c.get_population();
  }
  
}

ostream & operator<<(ostream &out, city &c) {
  // prints all city names to be city::w characters wide with ... filling in unused space where city::w is a static public member of the city 
  // class which is updated by create_vertex_table() when producing the list of largest county cities.
  // Specifically, city::w = max (city name length) + 3. The population field is 8 characters wide.
  // The latitude and longitude values are are both 7 characters wide with output restricted to two decimal places.
  // There are two blank spaces between each data item.

  out << std::setw(4) << std::right << c.index << "  " 
       << std::setfill('.') << std::setw(c.w) << std::left << c.get_cityName() << " "
       << std::setfill(' ') << std::setw(9) << std::right << c.get_population() << " "
       << std::fixed << std::setprecision(2)
       << std::setw(8) << (c.get_latitude() * RAD2DEG) << " "
       << std::setw(8) << (c.get_longitude() * RAD2DEG);

  return out;
}

//based upon https://www.geeksforgeeks.org/dsa/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
//got rid of the pow since they were giving me trouble and its quicker to do x*x then pow(x, 2)
float calculate_haversine(float lat1,float lon1, float lat2, float lon2){
  //returns the distance along a sphere between two points
  //3rd equation in the formulation chapter of the wiki page: 
  //    * hav(theta) = sin^2(dlat / 2) + (cos(lat1) * cos(lat2) * sin^2(dlon / 2))
  //d = 2r * archav(sqrt(hav(theta)))
  float dLat = lat2 - lat1;
  float dLon = lon2 - lon1;

  //apply the haversine formula
  float hav_theta = (sin(dLat / 2) * sin(dLat / 2)) + (cos(lat1) * cos(lat2)) * (sin(dLon / 2) * sin(dLon / 2));

  double d = 2 * asin(sqrt(hav_theta));
  
  return distance_accuracy * std::round((earthradius * d) / distance_accuracy); //miles rounded to the nearest 5.0 miles
}

template <typename datatype>
void create_vertex_table(string &fname, datatype &vertex_table) { //changed const char* to string & because it is easier to deal with
  //based upon lab1 void database::init(string datafile)
  //cities become vertices in a graph
  //Append the state name to the city name to produce "Knoxville_TN". The county should likewise become "Knox_TN"
  //Filter out cities whose population is less than 25,000. Determine the county city that has the largest population. 
  //Filter out all other cities. Designate cities with a population of 1,000,000 or more as hubs. When all data has been 
  //read, sort the largest county cities by population size, largest first and smallest last. Assign an incremental index 
  //to each city and use this for internal reference. The first part, where you determine the largest county cities, could 
  //be done by inserting the cities into a map which is sorted by county while using an overloaded += operatior to update 
  //the information when a larger population city is encounted. The second part, where you create the vertex table, could 
  //be done by copying the cities from the map to a vector whose content you then sort based on population. 
  //The hub designation could be implemented as a boolean function that returns true or false.

  std::ifstream fin;
  string line;
  int max_cityName_length = 0; //for formatting
  
  fin.open(fname); //we already error checked the file
  map<string, city> counties; //key: county name, value: county city that has the largest population
  // vector <city> hubs;
  while(getline(fin, line)){
    std::istringstream sin; //tokenize input 
    string cityName, countyName, state;
    float lat, lon;
    int pop;

    // change space to underscore
    for(char &s : line){
      if(s == ' '){  
        s = '_';
      }
    }
    
    // change comma to space
    for(char &s : line){
      if(s == ','){
        s = ' ';
      }
    }

    sin.str(line); //hold text to extrapulate data
    sin >> cityName >> countyName >> state >> lat >> lon >> pop;

    //filtering
    if(pop < city_minpop){
      continue; //skip to next city
    }

    //do the appendages
    cityName = cityName + "_" + state; //knoxville, ..., TN -> knoxville_TN
    countyName = countyName + "_" + state;

    //convert degrees to rads
    lat *= DEG2RAD;
    lon *= DEG2RAD;

    //store the city info
    city c;
    c.set_cityName(cityName);
    c.set_latitude(lat);
    c.set_longitude(lon);
    c.set_population(pop);
    max_cityName_length = std::max(((int)cityName.length()), max_cityName_length); //find which city will determine the max length
    
    //find county to get the city for it. Give a city to the county if there is no county in the map. Else update the county city when necessary
    if(counties.find(countyName) == counties.end()){
      counties[countyName] = c;
    }else{
      counties[countyName] += c;
    }
  }

  //https://www.geeksforgeeks.org/cpp/sorting-a-map-by-value-in-c-stl/
  //we will push the contents of counties to a vector to then be sorted
  vector <std::pair <string, city> > vertices;
  for(auto &it : counties){
    vertices.push_back(it);
  }

  auto cmp = [](std::pair<string, city> &a, std::pair<string, city> &b) {return a.second.get_population() > b.second.get_population(); }; 
  sort(vertices.begin(), vertices.end(), cmp); //vertices sorted by population

  for(int i = 0; i < (int)vertices.size(); i++){
    vertices[i].second.index = i; //second is the city
    vertices[i].second.w = max_cityName_length;
    vertex_table.push_back(vertices[i].second);
  }

  fin.close();
}
template <typename datatype>
void write_vertex_table(datatype &vertex_table) {
  // open vertex_table.txt and write to the file
  ofstream fout;
  fout.open("vertex_table.txt");
  if(!fout.is_open()){
    cerr << "usage: unable to open vertex_table.txt" << endl;
    fout.close();
    return;
  }
  
  for(auto &c : vertex_table){
    fout << c << endl;
  }
  
  fout.close();
}

//assume vertex_table has been created
template <typename datatype>
void create_dist_table(datatype &vertex_table, matrix &dist_table){
  //populate cell by cell with the value of the distances between the cities
  for(int row = 0; row < (int)dist_table.size; row++){
    for(int col = 0; col < (int)dist_table.size; col++){
      city fromCity = vertex_table[row];
      city toCity = vertex_table[col];
      float dist = calculate_haversine(fromCity.get_latitude(),
                                       fromCity.get_longitude(), 
                                       toCity.get_latitude(),
                                       toCity.get_longitude()); 
      
      //set the cell at the row, col to dist
      dist_table.set_value(row, col, dist);
    }
  }
}

template <typename datatype>
void create_time_table(datatype &vertex_table, matrix &dist_table, matrix &time_table){
  //populate cell by cell with the value of the time between cities
  //determine time between hubs by plane
  //other city pairs determined by truck 
  for(int row = 0; row < (int)dist_table.size; row++){
    for(int col = 0; col < (int)dist_table.size; col++){
      city fromCity = vertex_table[row];
      city toCity = vertex_table[col]; 
      
      //both cities have to be hubs (population >= 1,000,000) to be true 
      bool isHub = (fromCity.get_population() >= hub_minpop) && (toCity.get_population() >= hub_minpop); 

      float getTimeByPlane; //if hub, use plane speed else use truck speed
      float dist = dist_table.get_value(row, col);
      if(isHub && (dist >= hub_mindist)){
        getTimeByPlane = dist / plane_speed; 
      }else{
        getTimeByPlane = dist / truck_speed; 
      }

      //set the cell at the row, col to dist
      time_table.set_value(row, col, getTimeByPlane);
    }
  }
}

template <typename datatype1, typename datatype2>
void create_edge_table(datatype1 &vertex_table, datatype2 &edge_table, matrix &dist_table){
  // Cities are considered adjacent if they satisfy the condition for a Gabriel graph. As explained in class, 
  // this means that the circle defined by two cities does not contain a third city. Mathematically, you thus create an edge 
  // if dist(i,j)^2 < dist(i,k)^2 + dist(j,k)^2 where i, j and k are distinct indices. That is, i=[0:N], j=[0:i] and k != i or j. 
  // Hubs are adjacant if they satisfy the Gabriel condition and are at least 130 miles apart. You can implement this as 
  // two back-to-back triple-loop computations with the first loop processing all cities and the second loop processing hubs only. 
  // Since the end result is that cities are only adjacent to cities that are close to them, the edge table must be based on the 
  // list-of-lists implementation for sparsely connected graph

  float dist_IJ;
  float dist_IK;
  float dist_JK;

  //process all cities
  for(int row = 0; row < (int)vertex_table.size(); row++){ //process the three distances
    city i = vertex_table[row]; 

    for(int col = row + 1; col < (int)vertex_table.size(); col++){
      bool IJ_areAdjacent = true;
      city j = vertex_table[col];
      dist_IJ = dist_table.get_value(row, col); //distance between the city I and city J
      if(dist_IJ == 0){ //check if distance is 0
        continue;
      }

      //compare against each city K ensuring K != J || K != I
      for(int k = 0; k < (int)vertex_table.size(); k++){
        if((k == row) || (k == col)){
          continue;
        }

        dist_IK = dist_table.get_value(row, k);
        dist_JK = dist_table.get_value(col, k);

        if((dist_IJ * dist_IJ) >= ((dist_IK * dist_IK) + (dist_JK * dist_JK))){
          IJ_areAdjacent = false;
          break;
        }
      }

      if(IJ_areAdjacent){
        edge_table[i.get_cityName()].insert(j.index);
        edge_table[j.get_cityName()].insert(i.index);
      }
    }
  }

  //process hubs
  for(int row = 0; row < (int)vertex_table.size(); row++){ //process the three distances
    city i = vertex_table[row]; 
    if(i.get_population() < hub_minpop){
      continue; // not a hub
    }
    
    for(int col = row + 1; col < (int)vertex_table.size(); col++){
      bool IJ_areAdjacent = true;
      city j = vertex_table[col];
      if(j.get_population() < hub_minpop){
        break; // not a hub
      }

      dist_IJ = dist_table.get_value(row, col); //distance between the city I and city J
      if(dist_IJ < hub_mindist){ //check distance between I and J is at least 130 miles
        continue;
      }

      //compare against each city K ensuring K != J || K != I
      for(int k = 0; k < (int)vertex_table.size(); k++){
        city cityK = vertex_table[k];
        if(cityK.get_population() < hub_minpop){
          break; //don't check non hubs
        }

        if((k == row) || (k == col)){
          continue;
        }

        dist_IK = dist_table.get_value(row, k);
        dist_JK = dist_table.get_value(col, k);

        if((dist_IJ * dist_IJ) >= ((dist_IK * dist_IK) + (dist_JK * dist_JK))){
          IJ_areAdjacent = false;
          break;
        }
      }

      if(IJ_areAdjacent){
        edge_table[i.get_cityName()].insert(j.index);
        edge_table[j.get_cityName()].insert(i.index);
      }
    }
  }
}

template <typename datatype>
void write_dist_table(datatype &vertex_table, matrix &dist_table){
  //Implement these to write formatted versions of the data stored to file called dist_table.txt
  
  /*
    example output:
       217  Palm_City_FL............ to Madison_MS..............   695.0 miles
       217  Palm_City_FL............ to Starkville_MS...........   670.0 miles
       217  Palm_City_FL............ to Salem_VA................   700.0 miles
  */

  ofstream fout;
  fout.open("dist_table.txt");
  if(!fout.is_open()){
    fout.close();
    cerr << "Usage: unable to open dist_table.txt" << endl;
  }

  for(int row = 1; row < (int)vertex_table.size(); row++){
    city &from = vertex_table[row];
    
    for(int col = 0; col < (int)vertex_table.size(); col++){
      if(col == row){
        break;
      }

      city &to = vertex_table[col];
      fout << std::setw(4) << std::right << from.index << "  "
           << std::setfill('.') << std::setw(from.w) << std::left << from.get_cityName() << " to "
           << std::setw(from.w) << to.get_cityName() << " "
           << std::setfill(' ') << std::fixed << std::setprecision(1) << std::setw(7) << std::right << dist_table.get_value(from.index, to.index) << " miles"
           << endl;
    }
    fout << endl;
  }

  fout.close();
}

template <typename datatype>
void write_time_table(datatype &vertex_table, matrix &time_table){
  //Implement these to write formatted versions of the data stored to file called time_table.txt
  
  /*
    example output:
       217  Palm_City_FL............ to Madison_MS..............   695.0 miles
       217  Palm_City_FL............ to Starkville_MS...........   670.0 miles
       217  Palm_City_FL............ to Salem_VA................   700.0 miles
  */

  ofstream fout;
  fout.open("time_table.txt");
  if(!fout.is_open()){
    fout.close();
    cerr << "Usage: unable to open time_table.txt" << endl;
  }

  for(int row = 1; row < (int)vertex_table.size(); row++){
    city &from = vertex_table[row];
    
    for(int col = 0; col < (int)vertex_table.size(); col++){
      if(col == row){
        break;
      }

      city &to = vertex_table[col];
      fout << std::setw(4) << std::right << from.index << "  "
           << std::setfill('.') << std::setw(from.w) << std::left << from.get_cityName() << " to "
           << std::setw(from.w) << to.get_cityName() << " "
           << std::setfill(' ') << std::fixed << std::setprecision(1) << std::setw(7) << std::right << time_table.get_value(from.index, to.index) << " hours"
           << endl;
    }
    fout << endl;
  }

  fout.close();
}

template <typename datatype1, typename datatype2>
void write_edge_table(datatype1 &vertex_table, datatype2 &edge_table, matrix &dist_table, matrix &time_table){
  //Implement these to write formatted versions of the data stored to files called edge_table, respectively
  /*
    example output:
 217 Palm_City_FL
     23 Port_St_Lucie_FL........    10.0 miles  0.2 hours
     91 West_Palm_Beach_FL......    30.0 miles  0.5 hours
  */
  
  //write edge_table.txt
  ofstream fout;
  fout.open("edge_table.txt");
  if(!fout.is_open()){
    cerr << "usage: unable to open edge_table.txt" << endl;
    fout.close();
    return;
  }
  
  //print city c's edges with distance and time to get to j
  for(city &c : vertex_table){
    int i_edgeIdx = c.index;
    //header line
    fout << std::setw(4) << std::right << c.index << " " 
         << c.get_cityName() << endl; 
    
    //print the cities connected to the city in the header line
    for(int j_edgeIdx : edge_table[c.get_cityName()]){
      city &j = vertex_table[j_edgeIdx];
      fout << std::setw(6) << j.index << " " << std::setfill('.') << std::setw(c.w) << std::left << j.get_cityName() << " "
           << std::setfill(' ') << std::setprecision(1) << std::fixed << std::setw(7) << std::right << dist_table.get_value(i_edgeIdx, j_edgeIdx) << " miles  " //print distance
           << std::setw(2) << time_table.get_value(i_edgeIdx, j_edgeIdx) << " hours" //print time
           << endl;
    }
    
    fout << endl;
  }
  
  fout.close();
}

/*
  implementation based upon: 
    -https://favtutor.com/blogs/dijkstras-algorithm-cpp
    - spanning tree's How Dijkstra's Algorithm Works
      *https://www.youtube.com/watch?v=EFg3u_E6eHU
*/
template <typename datatype1, typename datatype2>
void dijkstra_route(int city_from, int city_to, 
                    datatype1 &vertex_table, datatype2 &edge_table,
                    int mode, matrix &dist_table, matrix &time_table)
{
  //determine the shortest path between two given cities either based on distance or time as specified the commandline argument
  //will handle the printing in this function as well

  matrix *selectedMatrix = NULL; //point to desired table depending on the mode
  string units; //will print miles or hours
  if(mode == DIST){ //using distance
    selectedMatrix = &dist_table;
    units = " miles";
  }

  if(mode == TIME){ //using time
    selectedMatrix = &time_table;
    units = " time";
  }

  //https://www.geeksforgeeks.org/cpp/priority-queue-in-cpp-stl/
  //use min heap to traverse the min weight
  typedef std::pair<float, int> node; //city index, weight
  std::priority_queue <node, vector<node>, std::greater<node> > pq; //initiate as min heap compared using float weight
  vector <float> weights(vertex_table.size(), std::numeric_limits <float>::infinity()); //assume the weights between cities are inifinity
  map <int, std::queue<int> > route_map; //will hold the current route used to get to city_to

  //set up source node data
  vertex_table[city_from].visited = GRAY; //set source as gray to then be set to black to avoid going back to the source
  weights[city_from] = 0.0f; //according to dijkstra's algorithm, we can conclude source to source has a weight 0
  pq.push({0.0f, city_from}); 
  route_map[city_from].push(city_from);

  //traverse each city at most twice to get the cost to get to city_to
  while(!pq.empty()){
    int idx = pq.top().second; //get the index at the top of the queue
    float currentCost = pq.top().first; 
    pq.pop(); 

    city &current = vertex_table[idx];
    
    //determine if the current city has been visited at most twice (black?)
    if(current.visited == BLACK){ //skip, the city has been visited twice
      continue;
    }
    
    if(current.visited == GRAY){ //visited once
      current.visited = BLACK; 
    }

    //set the city as visited once unless its the source
    if(current.visited == WHITE){
      current.visited = GRAY;
    }

    //skip if hit the target city or if the cost is greater than an established cost so there is no need to check its edges
    if((current.get_cityName() == vertex_table[city_to].get_cityName()) ||
       (currentCost > weights[city_to]))
    {
      continue;
    }

    //look at the edges of the current city to determine the order of pq
    for(int edgeIdx : edge_table[current.get_cityName()]){
      float weight = selectedMatrix->get_value(idx, edgeIdx);

      //determine if the weight is less than the value of the weight of that city in the edge_table and if that city has not been visited twice
      bool isVisitedOnce = vertex_table[edgeIdx].visited != BLACK;
      if(((currentCost + weight) < weights[edgeIdx]) && isVisitedOnce){
        //update the city's weight in weights and push the city in the priority queue
        weights[edgeIdx] = currentCost + weight; 
        pq.push({(currentCost + weight), edgeIdx,}); 

        //update the route map to the edge city
        route_map[edgeIdx] = route_map[idx]; //give the edge city the route to get to the current city
        route_map[edgeIdx].push(edgeIdx);
      }
    }
  }

  //print the route to city_to
  int prevIdx = route_map[city_to].front();
  while(!route_map[city_to].empty()){
    //get the city at the index of the what is at the top of the queue
    int idx = route_map[city_to].front();
    city &printingCity = vertex_table[idx];  
    route_map[city_to].pop();

    //printing the cost to get to this city and the city index and name
    cout << std::setfill(' ') << std::setprecision(2) << std::fixed << std::setw(8) << std::right << weights[idx] << units
         << std::setw(5) << printingCity.index << " "
         << std::setw(printingCity.w) << std::setfill('.') << std::left << printingCity.get_cityName();
    
    //print weight to the previous city
    if(idx == city_from){
      cout << endl; //ignore if the current city is the source
    }else{
      cout << std::setfill(' ') << std::setw(8) << std::right  
           << selectedMatrix->get_value(prevIdx, idx) << units << endl;
    }

    prevIdx = idx; //set the previous index
  }
  cout << endl;

  //at the end, reset each city's color back to white
  for(city &c : vertex_table){
    c.visited = WHITE;
  }

  selectedMatrix = NULL; //get rid of the pointer to the matrix
}

//usage: ./sprog6 -info|dist|time [-verbose] cities.txt           
int main(int argc, char *argv[]) {
  // parse commandline arguments
  if(argc != 3){
    cerr << "usage: " << argv[0] << "-info|dist|time cities.txt" << endl;
    return 1;
  }

  int mode = 0; //will which table we will create
  if(strcmp(argv[1], "-info") == 0){
    mode = INFO;
  }

  else if(strcmp(argv[1], "-dist") == 0){
    mode = DIST;
  }

  else if(strcmp(argv[1], "-time") == 0){
    mode = TIME;
  }

  else{
    cerr << "usage: " << argv[0] << "-info|dist|time cities.txt" << endl;
    return 1;
  }

  //set and error check the file
  string filename = argv[argc - 1]; //assume file is the last argument
  std::ifstream fin;
  fin.open(filename);
  if(!fin.is_open()){
    cerr << "usage: " << argv[0] << "-info|dist|time cities.txt" << endl;
    fin.close();
    return 1;
  }
  fin.close(); //only opening the file for error checking in main

  // create vertex_table, dist_table, time_table, and edge_table
  vector <city> vertex_table;
  create_vertex_table(filename, vertex_table);

  //create the matrices once we have a vertex vector
  matrix dist_table(vertex_table.size());
  matrix time_table(vertex_table.size());
  create_dist_table(vertex_table, dist_table);
  create_time_table(vertex_table, dist_table, time_table);

  typedef std::set <int>  citySet;
  map <string, citySet> edge_table; //key: cityName, value: indices of the cities in the edges
  create_edge_table(vertex_table, edge_table, dist_table);

  if(mode == 1){ //info mode
    write_vertex_table(vertex_table);
    write_edge_table(vertex_table, edge_table, dist_table, time_table);
    write_dist_table(vertex_table, dist_table);
    write_time_table(vertex_table, time_table);
    
    // time_table.print_table("test_time_mtrx.txt"); 
    // dist_table.print_table("test_dist_mtrx.txt"); 
    
    //exit if info mode
    return 0;
  }

  string input, city_from, city_to; //used to grab the user input
  while(true) {
    std::istringstream sin;
    int fromIdx = -1, toIdx = -1;
    cout << "Enter> "; 

    cin >> city_from >> city_to;
    if(cin.eof()){ //handle ctrl+d to terminate
      break;
    }

    //get the index of the cities
    for(city &c : vertex_table){
      if(c.get_cityName() == city_from){
        fromIdx = c.index;
      }

      if(c.get_cityName() == city_to){
        toIdx = c.index;
      }
    }

    //unable to find a inputted city in the vertex_table
    if(fromIdx == -1){
      cerr << city_from << ": prefix not found" << endl;
      continue;
    }

    if(toIdx == -1){
      cerr << city_to << ": prefix not found" << endl;
      continue;
    }

    dijkstra_route(fromIdx, toIdx, vertex_table, edge_table, mode, dist_table, time_table);
  }
  
  return 0;
}