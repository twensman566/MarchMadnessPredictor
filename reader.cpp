#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map> 
#include <sstream>

//#include <Eigen/Core>
//include <Eigen/Dense>
//using namespace Eigen;
using namespace std;


/*string removeSpaces(string str)
{
str.erase(remove(str.begin(), str.end(), ' ') str.end());
return str;
} */


/* Method to read in games file.
*
*@param gameFileName name of game file.
*
*
*/
void readInGames(string gameFileName){
	string date, team1, score1, team2, score2;
	string temp, line;
	fstream fout;
	int n = 6002;
	int i = 0;
	//MatrixXf matrix = MatrixXf(n,n);
	//VectorXf vector = VectorXf(n);
	fout.open("mcb2019CSV", fstream::in);
	
       	while (i<n){
	  // getline(fout, line);
		//	stringstream s(line);
		//cout<< line<< "\n";
		i++;
		getline(fout, date, ',');
		getline(fout, team1, ',');
		getline(fout, score1, ',');
		getline(fout, team2, ',');
		getline(fout, score2, '\n');
		cout << "Date:" << date << " Team1:  " << team1 << " Score1: " << score1 << " Team2: " << team2 << " Score2: " << score2 << "\n";
		//Put into matrix.
       	}
}

/*
* Method to read in team file.
*
*@param teamFileName name of team file.
*/
void readInTeams(string teamFileName){
	string num, team, line;
	fstream fout;
	fout.open("matlabteams.txt", ios::in);

	getline(fout, line);
	stringstream s(line);
	getline(s, num, ',');
	for (int i = 0; i <354; i++){
		getline(s, num, ',');
		int pos = num.find(" ");
		team = num.substr(0, pos);
		num = num.substr(pos);
		cout << team << " " << num << "\n";
	}
}

int main(int argc, char** argv){

	string teamFileName, gameFileName;
	cout << "Input name of team file\n";
	//	cin >> teamFileName;
	cout << "Input name of games file\n";
	//cin >> gameFileName;
	readInGames(gameFileName);
}
