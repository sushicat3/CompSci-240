#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//-------------------------------------------------------------
//  NOTES
//-------------------------------------------------------------
// - hypothesis: RUSSIAN, US, OTHER
// - observed data: A, B, C ...
// - priors: probabilities of RUSSIAN, US, OTHER based on prev data 
// - likelihood - probability of *CITY* containing A, B, C ...

string usName = "US";
string rusName = "Russian";
string othName = "Other";

// set of predictors
char LETTERS [] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
			    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

// subset of predictors for each object
int predictors [26];

// arrays holding likelihoods* of each predictor for each class: US, RUSSIA, OTHER.
// *actually holds the number of times a predictor was observed in the city data for each class.
// *likelihood = llh_class[i] / 100
int us [26];
int russia [26];
int other [26];

void initLlh(int llh[]) {
	for (int i = 0; i < 26; ++i)
		llh[i] = 0;
}

void initAll() {
	initLlh(us);
	initLlh(russia);
	initLlh(other);
}

//-------------------------------------------------------------
//  PREDICTOR FUNCTIONS
//-------------------------------------------------------------
// - records the predictors "observed" for each city name, in the global array predictors[]

void clearPredictors() {
	for (int i = 0; i < 26; ++i)
		predictors[i] = 0;
}

void observe( string city ) {
	clearPredictors();
	for (string::iterator it = city.begin(); it!=city.end(); ++it) {
		for (int i = 0; i < 26; ++i) {
			if ( (char) *it == LETTERS[i] ) {
				predictors[i] = 1;
			}
		}
	}
}

//-------------------------------------------------------------
//  LIKELIHOOD FUNCTIONS 
//-------------------------------------------------------------
//- calculates likelihoods for each predictor for the class of this file. 

void updateLlh(int llh[]) {
	for (int i = 0; i < 26; ++i) {
		llh[i] += predictors[i];
	}
}

void likelihood( int llh[], string filename ) {
	
	ifstream infile;

    infile.open (filename.c_str(), ifstream::in);

    string city;
    	while (infile.good()) {
    		getline(infile, city);
   	    	observe(city);
			updateLlh(llh);
		}

    infile.close();
}

//-------------------------------------------------------------
//  POSTERIORI FUNCTION
//-------------------------------------------------------------
double posteriori( string city, int arr[] ) {

	double prob = 0.5;

	observe(city);
	for (int i = 0; i < 26; ++i) {
		if (predictors[i] == 1) {
			prob *= ( (double) arr[i]/100.0 );
		}
		else {
			prob *= (1 - ( (double) arr[i]/100.0 ) );
		}
	}

	return prob;
}

//-------------------------------------------------------------
//  MAP HYPOTHETHIS 
//-------------------------------------------------------------
void genMAP( string ifn, string ofn, int arrTru[], int arrFal[], string nameTru, string nameFal ) {

	ifstream infile;
	ofstream outfile;

	double pstPROB_TRU, pstPROB_FAL, denom;
	bool acc = false;
	string nameMAP = nameTru;
	int acc_count = 0;
	int maxlength = 0;

    infile.open (ifn.c_str(), ifstream::in);
    outfile.open(ofn.c_str());

    string city;
    	while (infile.good()) {
    		getline(infile, city);

    		if (city.length() > maxlength) {
    			maxlength = city.length();
    		}

			pstPROB_TRU = posteriori(city, arrTru);
			pstPROB_FAL = posteriori(city, arrFal);
			denom = (pstPROB_TRU + pstPROB_FAL);

			if (pstPROB_TRU > pstPROB_FAL) acc = true;
			else acc = false;

			if (acc == true) {
				acc_count++;
				nameMAP = nameTru;
			}
			else nameMAP = nameFal;

			outfile << city 
						<< " | MAP: " << nameMAP 
						<< " | " << nameTru << " Posteriori: " << pstPROB_TRU/denom
						<< " | " << nameFal << " Posteriori: " << pstPROB_FAL/denom
						<< "\n";
		}

		outfile << "Accuracy of this test: " << (acc_count/50.0)*100 << '%' << "\n";

    infile.close();
    outfile.close();
}

int main() {

	initAll();

	// training:
	likelihood(russia, "russiaCities100.txt");
	likelihood(us, "usCities100.txt");
	likelihood(other, "otherCities100.txt");
	// prior will be 0.5 for each experiment.

	//void genMAP( string ifn, string ofn, int arrTru[], int arrFal[], string nameTru, string nameFal )
	//
	// - input file (the TRUE class)
	// - output file
	// - liklihood array of TRUE class
	// - likelihood array of FALSE class
	// - name of the TRUE class
	// - name of the FALSE class

	genMAP( "usCitiesNext50.txt", "exp1_USCities.txt", us, russia, usName, rusName);
	genMAP( "russiaCitiesNext50.txt", "exp1_RussiaCities.txt", russia, us, rusName, usName);

	genMAP( "russiaCitiesNext50.txt", "exp2_RussiaCities.txt", russia, other, rusName, othName);
	genMAP( "otherCitiesNext50.txt", "exp2_OtherCities.txt", other, russia, othName, rusName);

	genMAP( "usCitiesNext50.txt", "exp3_USCities.txt", us, other, usName, othName);
	genMAP( "otherCitiesNext50.txt", "exp3_OtherCities.txt", other, us, othName, usName);

    return 0;
}