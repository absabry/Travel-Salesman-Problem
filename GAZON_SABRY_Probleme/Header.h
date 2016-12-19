
typedef struct city* cities;
typedef struct city city;
struct city
{
    int key;
    char* name;
    double longitude;
    double latitude;
    struct city* next;
};

typedef struct cityMatrix cityMatrix;
struct cityMatrix
{
	int nbCity;
	double **tab;
};

typedef struct edge* edgeListe;
typedef struct edge edge;
struct edge
{
    int cityOneKey,cityTwoKey;
    double weight;
    struct edge* next;
};

typedef struct dictionnary* dictionnary;
typedef struct keysValue keysValue;
struct keysValue
{
    char* key;
    int value;
    struct keysValue* next;
};


// ToString
void edgesToString(edgeListe edges,cities theCities);
void cityToString(city *maCity);
void citiesToString(cities theCities);
void cityMatrixToString(cityMatrix* cityMat);
void dictionnaryToString(dictionnary dict);


// cities
city* addCity(int key,char* name,double longitude,double latitude,cities theCities);
char* getCityNameWithKey(int key,cities theCities);
int getCityKeyWithName(char* name,cities theCities);
city* getCity(char*name,cities theCities);
cities CreateCities(char* nameFile,int *nbCity);
cities deleteCity(cities theCities, int key);
city* getCityWithIndex(int index,cities theCities);
city* getCityWithKey(int key,cities theCities);
int getCityKeyWithName(char* nom,cities theCities);
cities cloneCities(cities original);
city* addTheCity(city* theCity,cities theCities);




// matrix
cityMatrix* intializeMatrix(int nbCity);
void stockBetweenCities(cityMatrix* cityMat, city* cityOne, city* cityTwo,double distance);
double getDistanceWithKeys(cityMatrix* cityMat,int cityOne, int cityTwo);
double getDistance(cityMatrix* cityMat, city* cityOne, city* cityTwo);
cityMatrix* AdjacentMatrix(cities theCities,int cardCity);

// secondary functions
int choixDeAlgo();
double computeTotalDistanceOfPath(cities theCities,cityMatrix* cityMat);
int stringEquals(char* one,char* two);


    // city
double calculDistance(city* one,city* two);
cities deleteCity(cities theCities, int key);
city* cloneCity(city* original,cities theCities);
cities convertTabToCities(char tab[100][100],int length,cities myCities);


    // kruskal secondary
edge* addEdgeSorted(int cityOneKey,int cityTwoKey,double weight,edgeListe edges);
edge* addEdge(int cityOneKey,int cityTwoKey,double weight,edgeListe edges);
keysValue* addKeysValue(char* keys,int value,dictionnary dictio);
int getValueOfKey(char* key,dictionnary dict);
dictionnary createDict(cities theCities);
dictionnary addToPath(dictionnary dict,int one,int two);
double Cost(edgeListe edges);
int citiesContainesCity(cities theCities, int keyOfCity);
int SpanningCompleted(dictionnary dict);


    // combine
void addCitesName(char chaine[100][100],cities theCities,char* villeDepart,int length);
void Combine(char*villeDepart,char chaine[100][100],int length,int limite,cityMatrix* cityMat,cities theCities);
void changePostion(char chaine[100][100],int cityOne,int cityTwo);

    // 20T
cities swapCities(city* first,city* second,cities theCities);
cities insertCity(int index,city* theCity,cities theCities);
cities deleteCity(cities theCities, int key);
cities replaceCityOfSource(char* nom,cities theCities,int nbCity);
int containsInfinityPath(cities theCities,cityMatrix* cityMat);

// algortihmes functions
void Kruskal(cityMatrix* Matrix,cities theCities,char* cityOne,char* cityTwo);
void createCombination(cities myCities,cityMatrix* cityMat,char* villeDepart,int length);
cities twoOPT(cities theCities,cityMatrix* cityMat,char* citySource);
cities localSearch(cityMatrix* cityMat,cities theCities,char *citySource);













