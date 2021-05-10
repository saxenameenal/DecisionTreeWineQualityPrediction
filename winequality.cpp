#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include "nodeClass.h"  // class file 
using namespace std;

// class variables are called from .h class file

// evaluating mode in data 
int mode(vector<vector<double> > &data){
   
    int five = 0, six = 0, seven = 0;
    
    for(int i = 0; i < data.size(); i++){
        if(data[i][11] == 5) five++;
        else if(data[i][11] == 6) six++;
        else if(data[i][11] == 7) seven++;
    }

    if (five > six && five > seven) return 5;
    if (six > five && six > seven) return 6;
    if (seven > five && seven > six) return 7;

    return 0;
}

// evaluating the if condition in algorithm 1 
bool yValue(vector <vector<double> > &data){
    for(int x = 0; x<data.size(); x++){
        if(data[x][11] != data[0][11]) return false;
    }
    return true;
}

bool xValue(vector <vector<double> > &data){
    for(int i = 1; i<data.size(); i++){
        for(int j = 0; j<11; j++){
            if(data[i][j] != data[0][j]) return false;
        }
    }
    return true;
}

// counting data in parent tree
int parentCount(vector<vector<double> > &data, int num){
    int count = 0;
    for (int i = 0; i<data.size(); i++){
        if(data[i][11] == num) count++;
    }
    return count;
}

//counting number of specific labels in quality column
int nCount(vector<vector<double> > &data, double &split, int attribute, int num, bool val){
    int count = 0;

    for(int i = 0; i<data.size(); i++){
        if(val == false){
            if(data[i][attribute] > split){
                if(data[i][11] == num ) count++;
            }
        }
        else if(val == true){
            if(data[i][attribute] <= split){
                if(data[i][11] == num ) count++;
            }
        }
    }

    return count;
}

// calculating probability 
double probabilityCalc(vector<vector<double> > &data, int num){
    double prob = ( (double)(num) / (double)(data.size()) );
    return prob;
}

int ChildprobabilityCalc(double &data, int num){
    double prob = ( (double)(num) / (double)(data) );
    return prob;
}

// calculating information content (i.e entropy) of parent, left and right children
double Entropy(vector<vector<double> > &data, int &five, int &six, int &seven, double count, bool val){

    double gain = 0, prob5 = 0, prob6 = 0, prob7 = 0;
    double sevenGain = 0, sixGain = 0, fiveGain = 0;

    //calculating parent information
    if(val == true ){ 
        
        prob7 = ( (double)(seven) / (double)(data.size()) );
        prob6 = ( (double)(six) / (double)(data.size()) ); 
        prob5 = ( (double)(five) / (double)(data.size()) );  
        
        // calculating gain through formula in lecture slide
        if(prob7 !=0) sevenGain = ( prob7*(log2(prob7)) ) ;
        if(prob6 !=0) sixGain = ( prob6*(log2(prob6)) ) ;
        if(prob5 !=0) fiveGain = ( prob5*(log2(prob5)) ) ;
        
        gain = ( -(sevenGain) - (sixGain) - (fiveGain) ) ;
    }

    // calculating right and left child information
    else if(val == false){
        
        // calculating probability, where count is total number of lefts and rights 
        if(count == 0) prob5 = 1, prob6 = 1, prob7 = 1;
    
        if(count != 0){
            prob7 = ( (double)(seven) / (double)(count) );
            prob6 = ( (double)(six) / (double)(count) );
            prob5 = ( (double)(five) / (double)(count) );
        }

        //calculating information content (entropy)
        if(prob7 == 0) sevenGain = 0;
        if(prob6 == 0) sixGain = 0;
        if(prob5 == 0) fiveGain = 0;

        if(prob7 != 0) sevenGain = ( prob7*(log2(prob7)) ) ;
        if(prob6 != 0) sixGain = ( prob6*(log2(prob6)) ) ;
        if(prob5 != 0) fiveGain = ( prob5*(log2(prob5)) ) ;
        
        gain = ( -(sevenGain) - (sixGain) - (fiveGain) ) ;       
    }
    
    return gain ;
}

// sorting data through bubble sort algorithm
int val = 12;
void swap(double *a, double *b ){
    int num = *a;
    *a = *b ;
    *b = num ; 
}

void sortData(vector <vector<double> > &data, int attr){
    for (int x = 0; x < data.size(); x++) {
        for (int y = 0; y < (data.size()-1); y++) {
            if( (data[y][attr]) < (data[y+1][attr]) ) {
                for(int z = 0; z < val ; z++) {
                    swap(data[y+1][z], data[y][z] ); 
                }
            } 
        }
    }
}

//algorithm 2 : calculating information gain and choosing split value
vector<double> chooseSplit(vector <vector<double> > &data){
    
    double bGain = 0, bSplitNum = 0, bAttr = 0, splitNum = 0;
    vector<double> returnVal;
 
    for(int attr = 0; attr < 11; attr++){
        
        sortData(data, attr);    

        for(int i = 0; i< data.size()-1 ; i++){

            // splitNum calculation             
            splitNum = 0.5*( (data[i][attr]) + (data[i+1][attr]) );

            // amount of labels in parent; right and left childs
            int parent5 = parentCount(data, 5);
            int parent6 = parentCount(data, 6);
            int parent7 = parentCount(data, 7);

            int right5 = nCount(data, splitNum, attr, 5, 0);
            int right6 = nCount(data, splitNum, attr, 6, 0);
            int right7 = nCount(data, splitNum, attr, 7, 0);

            int left5 = nCount(data, splitNum, attr, 5, 1);
            int left6 = nCount(data, splitNum, attr, 6, 1);
            int left7 = nCount(data, splitNum, attr, 7, 1);

            double rightCount = ( ((double)(right5)) + ((double)(right6)) + ((double)(right7)) );
            double leftCount = ( ((double)(left5)) + ((double)(left6)) + ((double)(left7)) );
            double stuff = 0;

            //entropy of parent; right and left childs
            double entropyParent = (double) (Entropy(data, parent5, parent6, parent7, stuff, 1) );
            double rightNodeEntropy = (double) (Entropy(data, right5, right6, right7, rightCount, 0) );
            double leftNodeEntropy = (double) (Entropy(data, left5, left6, left7, leftCount, 0));

            //probabilty of right and left childs 
            double probRightN = ( (double)(rightCount) / (double)(data.size()) );
            double probLeftN = ( (double)(leftCount) / (double)(data.size()) );

            // information gain calculation 
            double rightNodeSum = ( ((double)(probRightN)) * ((double)(rightNodeEntropy)) ) ;
            double leftNodeSum = ( ((double)(probLeftN)) * ((double)(leftNodeEntropy)) ) ; 
            double childSum = ( ((double)(rightNodeSum)) + ((double)(leftNodeSum)) ) ;
            double informationGain =  ((double)(entropyParent)) - ((double)(childSum)); 

            // finding best value that will be stored in vector and returned 
            if(informationGain > bGain){
                bGain = informationGain;
                bAttr = attr ;
                bSplitNum = splitNum;
            }
        }
    } 

    returnVal.push_back(bAttr);
    returnVal.push_back(bSplitNum);

    return returnVal;
}

// algorithm 1 : building the tree
node* dtl(vector <vector<double> > &data, int minleaf){

    bool x = xValue(data);
    bool y = yValue(data);
    int num = mode(data);
    int thing = data.size();
    //string const& stuff = "unknown";
    node* nNode = new node();
    vector <vector<double> > rightVec;
    vector <vector<double> > leftVec;

    //if condition being implemented
    if(x == true || y == true || thing <= minleaf){
        node* LeafNode = new node() ;
        if(num > 0 ) LeafNode -> label = num;
        //else LeafNode -> label = stuff; 
        return LeafNode;
    }

    vector <double> splitData = chooseSplit(data);

    // allocating node to the 2 values from choose split vector 
    nNode -> attr = splitData[0]; 
    nNode -> splitval = splitData[1];

    // holding split and attribute values 
    double attributeNum = splitData[0]; 
    double splitNum = splitData[1];     
    
    // storing data in correct vectors according to splitNum 
    for(int i = 0; i<data.size(); i++){

        if(data[i][attributeNum] > splitNum) rightVec.push_back(data[i]);

        else if(data[i][attributeNum] <= splitNum) leftVec.push_back(data[i]);
        
    }

    // recursive call for left and right children
    nNode -> rightNode = dtl(rightVec, minleaf);
    nNode -> leftNode = dtl(leftVec, minleaf);

    return nNode;
}

// algorithm 3 : predicting dtl 
int predictDTL(node *n, vector<double> &data){

    while(n->leftNode != 0 || n->rightNode != 0){

        if(data[n->attr] <= (n->splitval)) n = n->leftNode;
        else n = n->rightNode;

    }
    int result = n->label;
    cout<<result<<endl;
    return result;    

}

int main(int argc, char** argv){

    // storing data into matrix

    vector<vector<double> > traindata, testdata;

    // initialising variables to be used later 
    string data;
    string trainFile = argv[1];
    string testFile = argv[2];
    string minleafVal = argv[3];
    int minleaf = atoi(minleafVal.c_str()) ;
    
    node* hi = new node();
    
    ifstream trainData(trainFile);
    ifstream testData(testFile);     
    
    // initialising vectors and string to hold data values in 
    vector<double> FacidTrain;
    vector<double> VacidTrain;
    vector<double> CacidTrain;
    vector<double> ResSugarTrain;
    vector<double> ChloridesTrain;
    vector<double> FsDioxideTrain;
    vector<double> TsDioxideTrain;
    vector<double> DensityTrain;
    vector<double> pHTrain;
    vector<double> SulphatesTrain;
    vector<double> AlcoholTrain;
    vector<double> QualityTrain;

    string a, b, c, d, e, f, g, h, i, j, k, l;

    // storing each column into a row
    if(trainData){
        getline(trainData, data);
        while(!trainData.eof()){
            trainData >> a;
            FacidTrain.push_back(stof(a));

            trainData >> b;
            VacidTrain.push_back(stof(b));

            trainData >> c;
            CacidTrain.push_back(stof(c));

            trainData >> d;
            ResSugarTrain.push_back(stof(d));

            trainData >> e;
            ChloridesTrain.push_back(stof(e));

            trainData >> f;
            FsDioxideTrain.push_back(stof(f));

            trainData >> g;
            TsDioxideTrain.push_back(stof(g));

            trainData >> h;
            DensityTrain.push_back(stof(h));

            trainData >> i;
            pHTrain.push_back(stof(i));

            trainData >> j;
            SulphatesTrain.push_back(stof(j));

            trainData >> k;
            AlcoholTrain.push_back(stof(k));

            trainData >> l; 
            QualityTrain.push_back(stof(l));
        }

        // popping back extra value in vector
        FacidTrain.pop_back();
        VacidTrain.pop_back();
        CacidTrain.pop_back();
        ResSugarTrain.pop_back();
        ChloridesTrain.pop_back();
        FsDioxideTrain.pop_back();
        TsDioxideTrain.pop_back();
        DensityTrain.pop_back();
        pHTrain.pop_back();
        SulphatesTrain.pop_back();
        AlcoholTrain.pop_back();
        QualityTrain.pop_back();
    }

    vector<vector<double> > DataOne;

    // storing 1D vector into 2D big vector 
    DataOne.push_back(FacidTrain);
    DataOne.push_back(VacidTrain);
    DataOne.push_back(CacidTrain);
    DataOne.push_back(ResSugarTrain);
    DataOne.push_back(ChloridesTrain);
    DataOne.push_back(FsDioxideTrain);
    DataOne.push_back(TsDioxideTrain);
    DataOne.push_back(DensityTrain);
    DataOne.push_back(pHTrain);
    DataOne.push_back(SulphatesTrain);
    DataOne.push_back(AlcoholTrain);
    DataOne.push_back(QualityTrain);

    // transposing (flipping) matrix over to replicate exact data structure as train files 
    vector <vector<double> > Data1Final (DataOne[0].size(), vector<double>(DataOne.size()) );
    for (int i = 0; i < Data1Final.size(); i++){
        for (int j = 0; j < Data1Final[0].size(); j++){
            Data1Final[i][j] = DataOne[j][i];
        }
    }

    //Data1Final is final vector holding all training data

    trainData.close();

    // repeating above process again for test data 

    vector<double> FacidTest;
    vector<double> VacidTest;
    vector<double> CacidTest;
    vector<double> ResSugarTest;
    vector<double> ChloridesTest;
    vector<double> FsDioxideTest;
    vector<double> TsDioxideTest;
    vector<double> DensityTest;
    vector<double> pHTest;
    vector<double> SulphatesTest;
    vector<double> AlcoholTest;

    string aTest, bTest, cTest, dTest, eTest, fTest, gTest, hTest, iTest, jTest, kTest;

    if(testData){
        getline(testData, data);
        while(!testData.eof()){

            testData >> aTest;
            FacidTest.push_back(stof(aTest));

            testData >> bTest;
            VacidTest.push_back(stof(bTest));

            testData >> cTest;
            CacidTest.push_back(stof(cTest));

            testData >> dTest;
            ResSugarTest.push_back(stof(dTest));

            testData >> eTest;
            ChloridesTest.push_back(stof(eTest));

            testData >> fTest;
            FsDioxideTest.push_back(stof(fTest));

            testData >> gTest;
            TsDioxideTest.push_back(stof(gTest));

            testData >> hTest;
            DensityTest.push_back(stof(hTest));

            testData >> iTest;
            pHTest.push_back(stof(iTest));

            testData >> jTest;
            SulphatesTest.push_back(stof(jTest));

            testData >> kTest;
            AlcoholTest.push_back(stof(kTest));
        }

        FacidTest.pop_back();
        VacidTest.pop_back();
        CacidTest.pop_back();
        ResSugarTest.pop_back();
        ChloridesTest.pop_back();
        FsDioxideTest.pop_back();
        TsDioxideTest.pop_back();
        DensityTest.pop_back();
        pHTest.pop_back();
        SulphatesTest.pop_back();
        AlcoholTest.pop_back();
    }

    vector<vector<double> > DataTwo;

    DataTwo.push_back(FacidTest);
    DataTwo.push_back(VacidTest);
    DataTwo.push_back(CacidTest);
    DataTwo.push_back(ResSugarTest);
    DataTwo.push_back(ChloridesTest);
    DataTwo.push_back(FsDioxideTest);
    DataTwo.push_back(TsDioxideTest);
    DataTwo.push_back(DensityTest);
    DataTwo.push_back(pHTest);
    DataTwo.push_back(SulphatesTest);
    DataTwo.push_back(AlcoholTest);

    testData.close();

    vector <vector<double> > Data2Final (DataTwo[0].size(), vector<double>(DataTwo.size()) );

    for (int i = 0; i < Data2Final.size(); i++){
        for (int j = 0; j < Data2Final[0].size(); j++){
            Data2Final[i][j] = DataTwo[j][i];
        }
    }

    // setting node to dtl function algorithm 1
    hi = dtl(Data1Final, minleaf);
    int hello = ( Data2Final.size() );

    // calling predict function that outputs values learnt through train data 
    int num = 0;
    while (num < hello ){
        predictDTL(hi, Data2Final[num]);
        num++;
    }

    return 0;
}