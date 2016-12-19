#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Header.h"
#define LIGNESIZE 60
#define PI 3.1415926535
#define INFINI 999999.0

cityMatrix* intializeMatrix(int nbCity)
{
    int i = 0;
    cityMatrix *cityMat = NULL;
    cityMat = malloc(sizeof(cityMatrix));
    cityMat->tab = malloc(sizeof(double)*nbCity);
    for (i = 0; i < nbCity; i++) // foreach ligne i, add nbCity columns for having a matrix
    {
        *(cityMat->tab + i) = malloc(sizeof(double)*nbCity);
    }
    cityMat->nbCity=nbCity;
    return cityMat;
}

void stockBetweenCities(cityMatrix* cityMat, city* cityOne, city* cityTwo,double distance)//All edges are undirected, the adjacent matrix will be symetric
{
    cityMat->tab[cityOne->key][cityTwo->key]=distance;
    cityMat->tab[cityTwo->key][cityOne->key]=distance;
}

double getDistanceWithKeys(cityMatrix* cityMat,int cityOne, int cityTwo)//inputs : key
{
    return cityMat->tab[cityOne][cityTwo];
}

double getDistance(cityMatrix* cityMat, city* cityOne, city* cityTwo)// inputs : city
{
    return getDistanceWithKeys(cityMat,cityOne->key,cityTwo->key);
}

void cityMatrixToString(cityMatrix* cityMat) //Display the matrix
{
    int i = 0, j = 0;
    printf("\n");
    printf("\t");
    for(i=0;i<cityMat->nbCity;i++)
        printf("%d\t",i);
    printf("\n");
    for (i = 0; i < cityMat->nbCity; i++)
    {
        printf("%d\t",i);
        for (j = 0; j < cityMat->nbCity; j++)
        {
            double distance=getDistanceWithKeys(cityMat,i,j);
            if(distance==INFINI) printf("INFINI\t");
            else printf("%.2f\t", distance);
        }
        printf("\n");
    }
}

cityMatrix* AdjacentMatrix(cities theCities,int cardCity) // Crete the Adjacent Matrix
{
    cityMatrix *matrixCities=intializeMatrix(cardCity);
    city* parcoursOne=theCities;
    while(parcoursOne!=NULL)
    {
        city*parcoursTwo=parcoursOne;
        while(parcoursTwo!=NULL)
        {
            double distance=calculDistance(parcoursOne,parcoursTwo);
            if(distance<=100) //If the distance is lower than 100km -> INFINI
            {
                distance=INFINI;
            }
            stockBetweenCities(matrixCities,parcoursOne,parcoursTwo,distance);
            parcoursTwo=parcoursTwo->next;
        }
        parcoursOne=parcoursOne->next;
    }
    return matrixCities;
}

double calculDistance(city* one,city* two) //Calculate the distance between two cities
{
    double latFirst=one->latitude;
    double latSecond=two->latitude;
    double lonFirst=one->longitude;
    double lonSecond=two->longitude;
    double R = 6378.137;
    double dLat = latSecond * PI / 180 - latFirst * PI / 180;
    double dLon = lonSecond * PI / 180 - lonFirst * PI / 180;
    double a = sin(dLat/2) * sin(dLat/2) + cos(latFirst * PI / 180) * cos(latSecond * PI / 180) *sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double d = R * c;
    return d;
}

city* addCity(int key,char* name,double longitude,double latitude,cities theCities) // adding city to a list of citiesn, input:attribut of city
{
    city* newCity=malloc(sizeof(city));
    newCity->key=key;
    newCity->name=name;
    newCity->longitude=longitude;
    newCity->latitude=latitude;
    newCity->next=NULL;

    if(theCities==NULL) // first city of the list
    {
        return newCity;
    }
    else // add it on the end of the list
    {
        city* parcours= theCities;
        while(parcours->next!=NULL)parcours=parcours->next;
        parcours->next=newCity;
        return theCities;
    }
}

city* addTheCity(city* theCity,cities theCities) // adding city , input: another city
{
    city* newCity=cloneCity(theCity,NULL);
    if(theCities==NULL) // first element
    {
        return newCity;
    }
    else // add it in the end of the list
    {
        city* parcours= theCities;
        while(parcours->next!=NULL)parcours=parcours->next;
        parcours->next=newCity;
        return theCities;
    }
}

char* getCityNameWithKey(int key,cities theCities) // get the name of the city, given its key
{
    city* parcours=theCities;
    while(parcours!=NULL)
    {
        if(parcours->key==key) return parcours->name;
        parcours=parcours->next;
    }
    return NULL; // didint found this key in the list
}

void cityToString(city *maCity) // display a single city
{
    printf("%d :%s position :(%.2f;%.2f)\n",maCity->key,maCity->name,maCity->longitude,maCity->latitude);
}

void citiesToString(cities theCities) // display the list of cities
{
    city* parcours=theCities;
    while(parcours!=NULL)
    {
        cityToString(parcours);
        parcours=parcours->next;
    }
}

cities CreateCities(char* nameFile,int *nbCity) //convert the CSV file to list of cities
{
    FILE* file = fopen(nameFile, "r");
    cities Cities=NULL;
    if(file!=NULL)
    {
        char ligne [LIGNESIZE];
        double longitudeValue;
        double latitudeValue;
        char *nameValue;
        int countLignes=0;
        while (fgets (ligne, sizeof(ligne),file) != NULL ) // get each line
        {
            char* name = strtok (ligne,",");
            char* longitude = strtok (NULL, ",");
            char* latitude = strtok (NULL, ",");
            if(longitude !=NULL) // the function fgets has some bug, we have to put this conditon to continue
            {
                if(countLignes>0) // to avoid the first line (with titles)
                {
                    nameValue = malloc(strlen(name) + 1);
                    strcpy(nameValue, name);
                    longitudeValue=atof(longitude); //Convert char* to double
                    latitudeValue=atof(latitude);
                    Cities=addCity(countLignes-1,nameValue,longitudeValue,latitudeValue,Cities);
                }
                countLignes++;
            }
        }
        *nbCity=countLignes-1; // change the adresse of the nbCity (useful for other functions)
        fclose(file);
    }
    return Cities;
}

edge* addEdgeSorted(int cityOneKey,int cityTwoKey,double weight,edgeListe edges) // adding edges to list of edges, In ascending order
{
    edge* newEdge=malloc(sizeof(edge));
    newEdge->cityOneKey=cityOneKey;
    newEdge->cityTwoKey=cityTwoKey;
    newEdge->weight=weight;

    if(edges==NULL) // first edge
    {
        newEdge->next=NULL;
        return newEdge;
    }
    else // add it in the end of the list
    {
        if(edges->weight > newEdge->weight) // if the edge is smaller than the first edge of the list
        {
            newEdge->next=edges;
            return newEdge;
        }
        else
        {
            edge* parcours= edges;
            while(parcours->next!=NULL)
            {
                if(parcours->next->weight > newEdge->weight) // between the value smaller than him, and bigger than him
                {
                    edge* trasnfert=parcours->next;
                    parcours->next=newEdge;
                    newEdge->next=trasnfert;
                     return edges;
                }
                parcours=parcours->next;
            }
            parcours->next=newEdge;
            newEdge->next=NULL;
            return edges;
        }
    }
}

edge* addEdge(int cityOneKey,int cityTwoKey,double weight,edgeListe edges) // adding edges in unsorted way
{
    edge* newEdge=malloc(sizeof(edge));
    newEdge->cityOneKey=cityOneKey;
    newEdge->cityTwoKey=cityTwoKey;
    newEdge->weight=weight;

    if(edges==NULL)
    {
        newEdge->next=NULL;
        return newEdge;
    }
    else
    {
        edge* parcours=edges;
        while(parcours->next!=NULL)
            parcours=parcours->next;

        parcours->next=newEdge;
        newEdge->next=NULL;
        return edges;
    }
}

void edgesToString(edgeListe edges,cities theCities) // display edges
{
    edge* parcours=edges;
    while(parcours!=NULL)
    {
        printf("(%s;%s)=%.2f\n",getCityNameWithKey(parcours->cityOneKey,theCities),getCityNameWithKey(parcours->cityTwoKey,theCities),parcours->weight);
        parcours=parcours->next;
    }
}


void Kruskal(cityMatrix* Matrix,cities theCities,char* cityOne,char* cityTwo) //Kruskal
{
    int i,j,value1,value2;;
    edgeListe edges=NULL;
    edgeListe kruskal=NULL;
    dictionnary dict=createDict(theCities); //New dictionary, keys are the name of cities, values are the key of cities
    for(i=0;i<Matrix->nbCity;i++)
    {
        for(j=i+1;j<Matrix->nbCity;j++)
            edges=addEdgeSorted(i,j,getDistanceWithKeys(Matrix,i,j),edges); //Add of the edge between city i and j in ascending order
    }

    //Get the key of the two cities, compute their distance
    int key1=getCityKeyWithName(cityOne,theCities);
    int key2=getCityKeyWithName(cityTwo,theCities);
    double weight=getDistanceWithKeys(Matrix,key1,key2);
    kruskal=addEdge(key1,key2,weight,kruskal);
    dict=addToPath(dict,key1,key2);


    edge* parcours=edges;
    while(parcours!=NULL)
    {
        value1=getValueOfKey(getCityNameWithKey(parcours->cityOneKey,theCities),dict); // Get the Key from the Dictionary of the first city of this edge
        value2=getValueOfKey(getCityNameWithKey(parcours->cityTwoKey,theCities),dict); // The Key from the Dictionary of the second city of this edge
        weight=parcours->weight; //Calculate the distance between them
        if(value1!=value2) //If adding this edge won't create a cycle
        {
            kruskal=addEdge(parcours->cityOneKey,parcours->cityTwoKey,weight,kruskal);//Add the edge to result
            dict=addToPath(dict,value1,value2);//Add the city 1 to the path of the city 2
            if(SpanningCompleted(dict))
                break; //finish if the spanning tree is not completed
        }
        parcours=parcours->next;
    }
    edgesToString(kruskal,theCities);
    printf("cost=%.2f Kilometres\n",Cost(kruskal));
}

double Cost(edgeListe edges) //cost of the list egdes
{
    double cost=0;
    edge* parcours=edges;
    while(parcours!=NULL)
    {
        cost+=parcours->weight;
        parcours=parcours->next;
    }
    return cost;
}

keysValue* addKeysValue(char* keys,int value,dictionnary dictio) // add values to dictionary
{
    keysValue* dict=malloc(sizeof(keysValue));
    dict->key=keys;
    dict->value=value;
    dict->next=NULL;
    if(dictio==NULL)
    {
        return dict;
    }
    else
    {
        keysValue* parcours=dictio;
        while(parcours->next!=NULL)parcours=parcours->next;
        parcours->next=dict;
        return dictio;
    }

}

dictionnary addToPath(dictionnary dict,int one,int two) // edit the dictionnary, to have the same values (used in kursukal dictionnary)
{
    keysValue* parcours=dict;
    while(parcours!=NULL)
    {
        if(parcours->value==one)
        {
            parcours->value=two;
        }
        parcours=parcours->next;
    }
    return dict;
}

dictionnary createDict(cities theCities) // create dictionnary, using the name of cities n ahd their keys
{
    dictionnary dictio=NULL;
    city* parcours=theCities;
    while(parcours!=NULL)
    {
        dictio=addKeysValue(parcours->name,parcours->key,dictio);
        parcours=parcours->next;
    }
    return dictio;
}

void dictionnaryToString(dictionnary dict) // display dictionary
{
    keysValue* parcours=dict;
    while(parcours!=NULL)
    {
        printf("<%s;%d>\n",parcours->key,parcours->value);
        parcours=parcours->next;
    }
}

int getValueOfKey(char* key,dictionnary dict) // get value, given the key (name of the city)
{
    keysValue* parcours=dict;
    while(parcours!=NULL)
    {
        if(parcours->key==key) return parcours->value;
        parcours=parcours->next;
    }
}

int SpanningCompleted(dictionnary dict) // if all values in the dictionary are equals, then the spanning tree is complete
{
    keysValue* parcours=dict;
    int value=parcours->value;
    while(parcours!=NULL)
    {
        if(parcours->value!=value) return 0;
        parcours=parcours->next;
    }
    return 1;
}

void changePostion(char chaine[100][100],int cityOne,int cityTwo) // swap two values in some array
{
    char *temp;
    temp=malloc(sizeof(chaine[cityOne])+1);
    strcpy(temp,chaine[cityOne]);
    strcpy(chaine[cityOne],chaine[cityTwo]);
    strcpy(chaine[cityTwo],temp);
}

cities convertTabToCities(char tab[100][100],int length,cities myCities)
{
    int i;
    cities theCities=NULL;
    for(i=0;i<length;i++)
    {
        city* tmp=getCity(tab[i],myCities);
        theCities=addCity(tmp->key,tmp->name,tmp->longitude,tmp->latitude,theCities);
    }
    return theCities;
}

void Combine(char*villeSource,char chaine[100][100],int length,int limite,cityMatrix* cityMat,cities theCities)
{
    static double longestPath=0;
    static int counter=0;
    static char result[100][100];
    static cities resultat;
    int i,k;
    if (length-limite==1) // condition to stop the recursive functions
    {
        int j,cityOne,cityTwo;
        double distance=0,distanceAcutel;
        cities actual=convertTabToCities(chaine,length,theCities);

        // add source and destination to the list
        city*source=cloneCity(getCity(villeSource,theCities),NULL);
        actual=insertCity(0,source,actual);
        city*destination=cloneCity(getCity(villeSource,theCities),NULL);
        actual=insertCity(length+1,destination,actual);

        // compute distances
        distance=computeTotalDistanceOfPath(actual,cityMat);

        // update of the optimum result
        if(distance>longestPath)
        {
           longestPath=distance;
           resultat=cloneCities(actual);
        }
         // display the longest path
        printf("current result:\n");
        citiesToString(resultat);
        printf("longestPath:%.2f kilometres.",longestPath);
        printf("\n\n\n");
    }
    else // continue the recursive functions
    {
        for(i=limite;i<length;i++)
        {
            if(i!=limite)
            {
                changePostion(chaine,i,limite); // change the two values
                Combine(villeSource,chaine,length,limite+1,cityMat,theCities);
                changePostion(chaine,i,limite); // re-change it the two value
            }
            else // if i==limit, we dont have to swap
            {
                Combine(villeSource,chaine,length,limite+1,cityMat,theCities);
            }
        }
    }
}

void createCombination(cities myCities,cityMatrix* cityMat,char* villeSource,int length)  // main of combinations
{
    char cp[100][100];
    int i;
    char a[100][100];
    addCitesName(a,myCities,villeSource,length);
    for(i=0;i<length-1;i++)
        strcpy(cp[i],a[i]);
    Combine(villeSource,cp,length-1,0,cityMat,myCities);
}

void addCitesName(char chaine[100][100],cities theCities,char* villeSource,int length) // convert cities to char[][]
{
    int i=0;
    city* parcours=theCities;
    char*value;
    while(i<length-1)
    {
        int conditionAjout=stringEquals(villeSource,parcours->name);
        if(!conditionAjout)
        {
            value = malloc(strlen(parcours->name) + 1);
            strcpy(value, parcours->name);
            strcpy(chaine[i],value);
            i++;
        }
        parcours=parcours->next;
    }
}

int getCityKeyWithName(char* nom,cities theCities) // get key using name
{
    int bon=0;
    city* parcours=theCities;
    while(parcours!=NULL)
    {
        int i;
        bon=1;
        for(i=0;i<strlen(nom) && bon;i++)
        {
            if(parcours->name[i] != nom[i])
            {
                bon=0;
            }
        }
        if(bon) return parcours->key;
        parcours=parcours->next;
    }
    return -1;
}

int stringEquals(char* one,char* two)
{
    if(strlen(one)==strlen(two))
    {
        int i;
        for(i=0;i<strlen(one);i++)
        {
            if(one[i]!=two[i]) return 0;
        }
        return 1;
    }
    return 0;

}

int choixDeAlgo()
{
    int value=0;
    while(value!=1 && value!=2 && value!=3 && value!=4)
    {
        printf("\n\nWhich algorithme do you want to apply?\n1-All combinations\n2-Lin Keringhan\n3-Local search\n4-Spanning Tree\n");
        scanf("%d", &value);
    }
    return value;
}

cities deleteCity(cities theCities, int key)// input: key of the city
{
    if(theCities == NULL)
        return NULL;
    if(theCities->key == key)
    {
        city* temp = theCities->next;
        free(theCities);
        temp = deleteCity(temp, key);
        return temp;
    }
    else
    {
        theCities->next = deleteCity(theCities->next, key);
        return theCities;
    }
}

cities insertCity(int index,city* theCity,cities theCities) // insert city in some position of the list (first element is 0)
{
    if(index==0)
    {
        theCity->next=theCities;
        return theCity;
    }
    else
    {
        int i=0;
        city* parcours=theCities;
        while(i!=(index-1))
        {
            parcours=parcours->next;
            i++;
        }
        city* tmp=parcours->next;
        parcours->next=theCity;
        theCity->next=tmp;
        return theCities;
    }
}

cities swapCities(city* first,city* second,cities theCities) // useful for the 2OPT algortihme
{
    int indexActuel=0;
    if(first->key!=second->key)
    {
        city* parcours=theCities;
        while(parcours!=NULL)
        {
            if(parcours->key==second->key) // replace the second city with the first city
            {
                city* tmp=addCity(first->key,first->name,first->longitude,first->latitude,NULL);
                theCities=deleteCity(theCities,first->key);
                theCities=insertCity(indexActuel,tmp,theCities);
            }
            if(parcours->key==first->key)// replace the first city with the second city
            {
                city* tmp=addCity(second->key,second->name,second->longitude,second->latitude,NULL);
                theCities=deleteCity(theCities,second->key);
                theCities=insertCity(indexActuel,tmp,theCities);
            }
            indexActuel++;
            parcours=parcours->next;
        }
    }
    return theCities;
}

city* getCity(char*name,cities theCities) // return the city given its name
{
    city* parcours=theCities;
    while(parcours!=NULL)
    {
        if(stringEquals(parcours->name,name))return parcours;
        parcours=parcours->next;
    }
    return NULL;
}

city* getCityWithKey(int key,cities theCities) // return the city given its key
{
    city* parcours=theCities;
    while(parcours!=NULL)
    {
        if(parcours->key==key)return parcours;
        parcours=parcours->next;
    }
    return NULL;
}

city* getCityWithIndex(int index,cities theCities) // return city given its index
{
    int counter=0;
    city* parcours=theCities;
    while(parcours!=NULL)
    {
        if(index==counter) return parcours;
        parcours=parcours->next;
        counter++;
    }
    return NULL;
}

city* cloneCity(city* original,cities theCities)
{
    return addCity(original->key,original->name,original->longitude,original->latitude,theCities);
}

cities cloneCities(cities original) // clone cities
{
    city* result=NULL;
    city* parcours=original;
    while(parcours!=NULL)
    {
        result=cloneCity(parcours,result);
        parcours=parcours->next;
    }
    return result;
}

cities replaceCityOfSource(char* nom,cities theCities,int nbCity) // delete the source city, and add it in the bottom and the top
{
    cities newCities=cloneCities(theCities);
    city* oldOne=getCity(nom,theCities);
    city* top=cloneCity(oldOne,NULL);
    city* bottom=cloneCity(oldOne,NULL);
    newCities=deleteCity(theCities,oldOne->key);
    newCities=insertCity(0,top,newCities);
    newCities=insertCity(nbCity,bottom,newCities);
    return newCities;
}

cities twoOPT(cities theCities,cityMatrix* cityMat,char* citySource) // main 2OPT
{
    cities newCities=replaceCityOfSource(citySource,theCities,cityMat->nbCity);
    city* parcours=newCities->next;
    city*beforeParcours=newCities;
    int edited=1,counter=1;
    while(counter<=cityMat->nbCity && edited==1) // for n^2 complexity in worst case
    {
        parcours=newCities->next;
        beforeParcours=newCities;
        edited=0;
        while(parcours->next->next!=NULL) // we cannot change PARIS (The last one in the list)
        {
            double oldDistance=getDistance(cityMat,beforeParcours,parcours)+getDistance(cityMat,parcours->next,parcours->next->next);
            double newDistance=getDistance(cityMat,beforeParcours,parcours->next)+getDistance(cityMat,parcours,parcours->next->next);
            if(oldDistance>=INFINI && newDistance<INFINI) // first one have a infinty edge,after swap its gone
            {
                char* nextValue=parcours->next->name;
                newCities=swapCities(parcours,parcours->next,newCities);
                beforeParcours=getCity(nextValue,newCities);
                edited=1;
            }
            else if(newDistance <= INFINI) // the swap is creating a infinty edge
            {
                if(oldDistance<newDistance) // if the swap is useful
                {
                    char* nextValue=parcours->next->name;
                    newCities=swapCities(parcours,parcours->next,newCities);
                    beforeParcours=getCity(nextValue,newCities);
                    edited=1;
                }
                else // the swap isnt useful
                {
                    beforeParcours=parcours;
                    parcours=parcours->next;
                }
            }
            else // nothing happens
            {
                beforeParcours=parcours;
                parcours=parcours->next;
            }
         }
         counter++;
    }

    if(getDistance(cityMat,parcours,parcours->next)==INFINI)
    {
        printf("KO,the last city is less than 100 km away from Paris\n");
        return NULL;
    }
    else if(containsInfinityPath(newCities,cityMat))
    {
        printf("KO,there are a infinity path between the cities\n");
        return NULL;
    }
    else
    {
        citiesToString(newCities);
        return newCities;
    }
}

int citiesContainesCity(cities theCities, int keyOfCity)
{
    city* parcours=theCities;
    while(parcours!=NULL)
    {
        if(parcours->key==keyOfCity) return 1;
        parcours=parcours->next;
    }
    return 0;
}

cities localSearch(cityMatrix* cityMat,cities theCities,char *citySource) // local search main
{
    int keyCityParcours,keyCityParcoursBis,i;
    cities theCitiesModified=replaceCityOfSource(citySource,theCities,cityMat->nbCity); //  PARIS delete it form the list in the begin and the end
    city*optimum=cloneCity(theCitiesModified,NULL);
    cities constructed=NULL;
    constructed=addTheCity(optimum,constructed); // add paris to the begin of the result list
    city*parcours=constructed;
    city*parcoursBis=NULL;
    for(i=0;i<cityMat->nbCity-1;i++) // we will have a list that contains nbCity elemtns (+the first city PARIS)
    {
        keyCityParcours=getCityKeyWithName(parcours->name,theCities);
        parcoursBis=theCitiesModified->next;
        double maxValue=0;
        while(parcoursBis->next!=NULL)
        {
            keyCityParcoursBis=getCityKeyWithName(parcoursBis->name,theCities);
            if(keyCityParcours!=keyCityParcoursBis && !citiesContainesCity(constructed,keyCityParcoursBis)) // not the same city AND didnt added to tne result value
            {
                double currentValue=getDistanceWithKeys(cityMat,keyCityParcours,keyCityParcoursBis);
                if(currentValue>maxValue && currentValue!=INFINI)
                {
                    optimum=cloneCity(parcoursBis,NULL);
                    maxValue=currentValue;
                }
            }
            parcoursBis=parcoursBis->next;
        }
        constructed=addTheCity(optimum,constructed);
        parcours=parcours->next;
    }
    // Add Paris to the end of the path
    optimum=cloneCity(theCitiesModified,NULL);
    constructed=addTheCity(optimum,constructed);
    if(getDistance(cityMat,parcours,parcours->next)!=INFINI)
    {
        citiesToString(constructed);
        return constructed;
    }
    else
    {
       printf("KO,the last city is less than 100 km away from Paris\n");
       return NULL;
    }
}

double computeTotalDistanceOfPath(cities theCities,cityMatrix* cityMat)
{
    double result=0;
    city*parcours=theCities;
    while(parcours->next!=NULL)
    {
        int getKeyCityOne=getCityKeyWithName(parcours->name,theCities);
        int getKeyCityTwo=getCityKeyWithName(parcours->next->name,theCities);
        result+=getDistanceWithKeys(cityMat,getKeyCityOne,getKeyCityTwo);
        parcours=parcours->next;
    }
    return result;
}

int containsInfinityPath(cities theCities,cityMatrix* cityMat)
{
    city*parcours=theCities;
    while(parcours->next!=NULL)
    {
        int getKeyCityOne=getCityKeyWithName(parcours->name,theCities);
        int getKeyCityTwo=getCityKeyWithName(parcours->next->name,theCities);
        if(getDistanceWithKeys(cityMat,getKeyCityOne,getKeyCityTwo)==INFINI)
        {
            return 1;
        }
        parcours=parcours->next;
    }
    return 0;
}

int main()
{
    int choix;
    cities myCities=NULL;
    int cardCities;
    printf("\n\nYou can use the file Cites.csv or the file CitesTEST.csv to run algortihmes\nThe difference is the number of cities in the file.\n\n\n");
    myCities=CreateCities("Cites.csv",&cardCities);
    citiesToString(myCities);
    cityMatrix* matrix=AdjacentMatrix(myCities,cardCities);
    //cityMatrixToString(matrix);
    choix=choixDeAlgo();
    switch(choix)
    {
        case 1:
            createCombination(myCities,matrix,"PARIS",cardCities);
            break;
        case 2:
            myCities=twoOPT(myCities,matrix,"PARIS");
            if(myCities!=NULL) printf("%.2f Kilometres",computeTotalDistanceOfPath(myCities,matrix));
            break;
        case 3:
            myCities=localSearch(matrix,myCities,"PARIS");
            if(myCities!=NULL) printf("%.2f Kilometres",computeTotalDistanceOfPath(myCities,matrix));
            break;
        case 4:
            Kruskal(matrix,myCities,"PARIS","SAINT GEORGES");
            break;
    }
}






