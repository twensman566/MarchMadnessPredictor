#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map> 
#include <string>
#include <string.h>
#include <sstream>
#include <bits/stdc++.h>

//#include <Eigen/Eigen/Core>
//#include <Eigen/Eigen/Dense>
//using namespace Eigen;
using namespace std;

/*
 *GLOBAL VARIABLES
 *
 */
//Eigen::MatrixXi m(353,353);



/* Method to read in games file.
*
*@param gameFileName name of game file.
*
*
*/
void readInGames(string gameFileName,map<string,int> map){
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
		
		//cout<<map.find(team2)->first<<map.find(team2)->second<<"\n";
		cout << "Date:" << date << " Team1:  " << team1 << " Score1: " << score1 << " Team2: " << team2 << " Score2: " << score2 << "\n";
		//Put into matrix.
       	}
}

/*
 *Method to remove underscores from team name to match game names.
 *@param teamName the name of the team to edit.
 *
 *
 */
string newTeamName(string teamName){
  /*string u = "_";
		for(int j=0;j < teamName.length();j++){
		  if(teamName.substr(j,j+1)=="_"){
		    teamName = teamName.substr(0,j)+" "+teamName.substr(j);
		  }
		  }*/
  string temp = teamName;
  char array[1024];
  strcpy(array,temp.c_str());
  for(int i = 0;i < teamName.length();i++){
    if(array[i]=='_'){
      array[i]=' '; 
    }
  }
  string newName(array);
  return newName;
}


/*
* Method to read in team file.
*
*@param teamFileName name of team file.
*/
map<string,int> readInTeams(string teamFileName){
        string num,line;
	fstream fout;
	fout.open("mcb2019teams", ios::in);
	map<string,int> map;
	//EXAMPLE map.insert(pair<string,int>("Me",1));
	//        cout<<map.find("Me")->second<<"\n";
	getline(fout, line);
	stringstream s(line);
	getline(fout, num, ',');
	for (int i = 0; i <353; i++){
		getline(s, num, ',');
      		string team = num;
		team = team.substr(1);
		int pos = team.find(" ",0);
		string name = team.substr(0,pos);
		string teamNum = team.substr(pos+1);
		string teamName = newTeamName(name);
		//cout << teamName <<" "<< teamNum<<"\n";
		int n = atoi(teamNum.c_str());
		map.insert(pair<string,int>(teamName,n-2));
		
	}
	return map;
}



int main(int argc, char** argv){

	string teamFileName, gameFileName;
	cout << "Input name of team file\n";
	//	cin >> teamFileName;
	cout << "Input name of games file\n";
	//cin >> gameFileName;
	// READ IN TEAMS
	map<string,int> map = readInTeams(teamFileName);
        cout<<map.find("Duke")->first<<" "<< map.find("Duke")->second<<"\n";
	//READ IN GAMES
	readInGames(gameFileName,map);
}
