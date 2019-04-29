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

Eigen::MatrixXd matrix = Eigen::MatrixXd::Zero(267,32);
Eigen::MatrixXd score = Eigen::MatrixXd::Zero(267,1);
Eigen::MatrixXd matrix2 = Eigen::MatrixXd::Zero(32,32);
Eigen::MatrixXd rightHandSide = Eigen::MatrixXd::Zero(32,1);
Eigen::MatrixXd rMatrix = Eigen::MatrixXd::Zero(32,1);//RATINGS


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
  int n = 267;
  int i = 0;
  int team1Home = 0;
  int team2Home = 0;
  //MatrixXf matrix = MatrixXf(n,n);
  //VectorXf vector = VectorXf(n);
  fout.open("NFLGames2018", fstream::in);
	
  while (i<n){
    // getline(fout, line);
    //	stringstream s(line);
    //cout<< line<< "\n";
    getline(fout, date, ',');
    getline(fout, team1, ',');
    getline(fout, score1, ',');
    getline(fout, team2, ',');
    getline(fout, score2, ',');
    if(checkAway(team1)){
      team1 = team1.substr(1);
      team1Home = .2;
    }
    else if(checkAway(team2)){
      team2 = team2.substr(1);
      team2Home = .2;
    }
    //cout<<map.find(team2)->first<<map.find(team2)->second<<"\n";
    //cout << "Date:" << date << " Team1:  " << team1 << " Score1: " << score1 << " Team2: " << team2 << " Score2 " << score2 << "\n";
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
      if(winner>0&&winner<32&&loser>0&&loser<32){
	matrix(i,winner)= 1 +team1Home;
	matrix(i,loser) = -1-team1Home;
	score(i,0)=diff;
      }
    }
    else{
      diff = scoreII - scoreI;
      int winner = map.find(team2)->second;
      int loser = map.find(team1)->second;
      if(winner>0&&winner<32&&loser>0&&loser<32){
	matrix(i,winner)= 1+team2Home;
	matrix(i,loser) = -1-team2Home;
	score(i,0)=diff;
      }
    }
    i++;
    team1Home = 0;
    team2Home = 0;
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



map<string,int> readInTeams(string teamFileName){
        string num,line;
	fstream fout;
	fout.open("NFLTeams", ios::in);
	map<string,int> map;
	//EXAMPLE map.insert(pair<string,int>("Me",1));
	//        cout<<map.find("Me")->second<<"\n";
	//getline(fout, line);
	//stringstream s(line);
	//getline(fout, num, ',');
	for (int i = 0; i <32; i++){
	  
		getline(fout, num);
      		string team = num;
		//cout<<team<<"\n";
		//team = team.substr(1);
		int pos = team.find(",",0);
		string name = team.substr(pos+1);
		string teamNum = team.substr(0,pos);
		string teamName = newTeamName(name);
		//cout << teamName <<" "<< teamNum<<"\n";
		int n = atoi(teamNum.c_str());
		map.insert(pair<string,int>(teamName,n-1));
		cout<< teamName<< " "<< n-1 << "\n";
	}
	return map;
}


void combine(){
  matrix2 = matrix.transpose()*matrix;
  rightHandSide = matrix.transpose()*score;
  //cout << matrix2;
  //cout << rightHandSide<<"\n";
  for(int i = 0;i<32;i++){
    matrix2(31,i) = 1;
  }
  rightHandSide(31,0) = 0;
  rMatrix = ((matrix2.transpose()*matrix2).inverse())*matrix2.transpose()*rightHandSide;
}


/*
 * Method that returns the highest rating amongst all the teams.
 *
 */
void highestRating(map<string,int> map){
  double highest = rMatrix(0,0);
  int index = 0;
  string highSchool;
  std::map<string,int>::iterator it = map.begin();
  int i = 0;
  while(it != map.end()){
    if(rMatrix(i,0)>highest){
      highest =rMatrix(i,0);
      highSchool = it->first;
    }
    it++;
    i++;
  }
  cout<<"Highest rating is "<<highSchool<<" with a rating of "<<highest<<"\n";
}


/*
 *Method to print out ratings for each team.
 *
 */

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
	cout << "Welcome to the Tetrominoes NFL 2018 ranking system\n";
	// READ IN TEAMS
	map<string,int> map = readInTeams(teamFileName);
	//READ IN GAMES
	readInGames(gameFileName,map);
	//Combine Matrices to get ranking values;
	combine();
	//Prints off all ratings
	cout<< "Each teams rating.\n";
	printRatings(map);
	//Prints off highest rating.
	cout<< "The highest rating is: \n";
	highestRating(map);
	//test();
	//firstRound(map);
}
