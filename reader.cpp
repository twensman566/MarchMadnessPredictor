#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map> 
#include <string>
#include <string.h>
#include <sstream>
#include <bits/stdc++.h>
#include <iterator>

#include <Eigen/Eigen/Core>
#include <Eigen/Eigen/Dense>
//using namespace Eigen;
using namespace std;

/*
 *GLOBAL VARIABLES
 *
 */
Eigen::MatrixXd matrix(6002,353);
Eigen::MatrixXd score(6002,1);
Eigen::MatrixXd matrix2(353,353);
Eigen::MatrixXd rightHandSide(353,1);
Eigen::MatrixXd rMatrix(353,1);



/*
 *Checks if a team was away.
 *
 *
 *
 */
bool checkAway(string teamName){
  string temp = teamName;
  char array[1024];
  strcpy(array,temp.c_str());
  if(array[0]=='@'){
    return true;
  }
  return false;

}

/* Method to read in games file.
*
*@param gameFileName name of game file.
*add 1 to diagonal of team that won
*add -1 to corresponding column of team that lost
*X(T)Xr = X(T)y
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
    getline(fout, date, ',');
    getline(fout, team1, ',');
    getline(fout, score1, ',');
    getline(fout, team2, ',');
    getline(fout, score2, '\n');
    if(checkAway(team1)){
      team1 = team1.substr(1);
    }
    else if(checkAway(team2)){
      team2 = team2.substr(1);
    }
    //cout<<map.find(team2)->first<<map.find(team2)->second<<"\n";
    //cout << "Date:" << date << " Team1:  " << team1 << " Score1: " << score1 << " Team2: " << team2 << " Score2: " << score2 << "\n";
    int scoreI = atoi(score1.c_str());
    int scoreII = atoi(score2.c_str());
    int diff;
    //Winning
    if(scoreI>scoreII){
      diff = scoreI - scoreII;
      matrix(i,0)=diff;
      int winner = map.find(team1)->second;
      int loser = map.find(team2)->second;
      //cout<< winner<< " " << i<<"\n";
      if(winner>0&&winner<353&&loser>0&&loser<353){
	matrix(i,winner)= 1;
	matrix(i,loser) = -1;
	score(i,0)=diff;
      }
    }
    else{
      diff = scoreII - scoreI;
      int winner = map.find(team2)->second;
      int loser = map.find(team1)->second;
      if(winner>0&&winner<353&&loser>0&&loser<353){
	matrix(i,winner)= 1;
	matrix(i,loser) = -1;
	score(i,0)=diff;
      }
    }
    i++;
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

void combine(){
  matrix2 = matrix.transpose()*matrix;
  rightHandSide = matrix.transpose()*score;
  //cout << matrix2;
  //cout << rightHandSide<<"\n";
  rMatrix = ((matrix2.transpose()*matrix2).inverse())*matrix2.transpose()*rightHandSide;
}

void printRatings(map<string,int> map){
  std::map<string,int>::iterator it = map.begin();
  while(it != map.end()){
    int spot = it->second;
    cout<<it->first<< " " <<rMatrix(spot,0)<<"\n";
    it++;
  }
}

int main(int argc, char** argv){

	string teamFileName, gameFileName;
	cout << "Input name of team file\n";
	//	cin >> teamFileName;
	cout << "Input name of games file\n";
	//cin >> gameFileName;
	// READ IN TEAMS
	map<string,int> map = readInTeams(teamFileName);
        //cout<<map.find("Duke")->first<<" "<< map.find("Duke")->second<<"\n";
	//READ IN GAMES
	readInGames(gameFileName,map);
	combine();
	printRatings(map);
}
