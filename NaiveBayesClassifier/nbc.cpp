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

char obs [] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
			    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

int hasLet [26];

int us_llh [26];
int russia_llh [26];
int other_llh [26];

void initLlh(int arr[]) {
	for (int i = 0; i < 26; ++i)
		arr[i] = 1;
}

//-------------------------------------------------------------
//  HAS LETTER FUNCTIONS
//-------------------------------------------------------------
void hasLetFcn( string city ) {
	for (string::iterator it = city.begin(); it!=city.end(); ++it) {
		for (int i = 0; i < 26; ++i) {
			if ( (char) *it == obs[i] ) {
				hasLet[i] = 1;
			}
		}
	}
}

void clrHasLet() {
	for (int i = 0; i < 26; ++i)
		hasLet[i] = 0;
}

//-------------------------------------------------------------
//  LIKELIHOOD FUNCTIONS 
//-------------------------------------------------------------
void incLlh(int arr[]) {
	for (int i = 0; i < 26; ++i) {
		arr[i] += hasLet[i];
	}
}

void llhFcn( int arr[], string filename ) {
	
	ifstream infile;

    infile.open (filename.c_str(), ifstream::in);

    string city;
    	while (infile.good()) {
    		getline(infile, city);
   	    	hasLetFcn(city);
			incLlh(arr);
			clrHasLet();
		}

    infile.close();
}

//-------------------------------------------------------------
//  POSTERIOR FUNCTION
//-------------------------------------------------------------
double pstrFcn( string city, int arr[] ) {

	double prob = 0.5;

	hasLetFcn(city);
	for (int i = 0; i < 26; ++i) {
		if (hasLet[i] == 1) {
			prob *= ( (double) arr[i]/126.0 );
		}
	}
	clrHasLet();

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


    infile.open (ifn.c_str(), ifstream::in);
    outfile.open(ofn.c_str());

    string city;
    	while (infile.good()) {
    		getline(infile, city);

			pstPROB_TRU = pstrFcn(city, arrTru);
			pstPROB_FAL = pstrFcn(city, arrFal);
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

	initLlh(us_llh);
	initLlh(russia_llh);
	initLlh(other_llh);

	llhFcn(russia_llh, "russiaCities100.txt");
	llhFcn(us_llh, "usCities100.txt");
	llhFcn(other_llh, "otherCities100.txt");

	//void genMAP( string ifn, string ofn, int arrTru[], int arrFal[], string nameTru, string nameFal )
	//
	// - input file (the TRUE class)
	// - output file
	// - liklihood array of TRUE class
	// - likelihood array of FALSE class
	// - name of the TRUE class
	// - name of the FALSE class

	genMAP( "usCitiesNext50.txt", "exp1_USCities.txt", us_llh, russia_llh, usName, rusName);
	genMAP( "russiaCitiesNext50.txt", "exp1_RussiaCities.txt", russia_llh, us_llh, rusName, usName);

	genMAP( "russiaCitiesNext50.txt", "exp2_RussiaCities.txt", russia_llh, other_llh, rusName, othName);
	genMAP( "otherCitiesNext50.txt", "exp2_OtherCities.txt", other_llh, russia_llh, othName, rusName);

	genMAP( "usCitiesNext50.txt", "exp3_USCities.txt", us_llh, other_llh, usName, othName);
	genMAP( "otherCitiesNext50.txt", "exp3_OtherCities.txt", other_llh, us_llh, othName, usName);

    return 0;
}