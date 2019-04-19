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
Eigen::MatrixXd matrix = Eigen::MatrixXd::Zero(6002,353);
Eigen::MatrixXd score = Eigen::MatrixXd::Zero(6002,1);
Eigen::MatrixXd matrix2 = Eigen::MatrixXd::Zero(353,353);
Eigen::MatrixXd rightHandSide = Eigen::MatrixXd::Zero(353,1);
Eigen::MatrixXd rMatrix = Eigen::MatrixXd::Zero(353,1);



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
* Method to read in team file and returns a map of the team name (key) and place in matrix (value).
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

/*
 *Method to do the matrix calculations.
 *
 */
void combine(){
  matrix2 = matrix.transpose()*matrix;
  rightHandSide = matrix.transpose()*score;
  //cout << matrix2;
  //cout << rightHandSide<<"\n";
  for(int i = 0;i<353;i++){
    matrix2(352,i) = 1;
  }
  rightHandSide(352,0) = 0;
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
      it++;
      i++;
    }
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


/*
 *Test matrix to verify we get the right values.
 *
 */
void test(){
  Eigen::MatrixXd xMat = Eigen::MatrixXd::Zero(5,4);// xMat(5,4);
  cout<<xMat<<"\n";
  xMat(0,0)=1;
  xMat(0,1)=-1;
  //xMat(0,2)=0;
  //xMat(0,3)=0;

  // xMat(1,0)=0;
  //xMat(1,1)=0;
  xMat(1,2)=1;  
  xMat(1,3)=-1;
  
  //xMat(2,0)=0;
  xMat(2,1)=-1;
  //xMat(2,2)=0;
  xMat(2,3)=1;
  
  xMat(3,0)=1;
  //xMat(3,1)=0;
  //xMat(3,2)=0;
  xMat(3,3)=-1;
  
  //xMat(4,0)=0;
  xMat(4,1)=1;
  xMat(4,2)=-1;
  //xMat(4,3)=0;
  
  Eigen::MatrixXd yMat(5,1);
  yMat(0,0)=4;
  yMat(1,0)=0;
  yMat(2,0)=7;
  yMat(3,0)=2;
  yMat(4,0)=1;
  cout<<xMat<<"\n\n";
  cout<<yMat<<"\n\n";
  
  
  Eigen::MatrixXd x2 = Eigen::MatrixXd::Zero(4,4);
  Eigen::MatrixXd y2 = Eigen::MatrixXd::Zero(4,1);
  Eigen::MatrixXd gj = Eigen::MatrixXd::Zero(4,1);
  x2 = xMat.transpose()*xMat;
  cout<<x2<<"\n\n";
  y2 = xMat.transpose()*yMat;
  cout<<y2<<"\n\n";
  //cout << matrix2;
  //cout << rightHandSide<<"\n";
  x2(3,0) = 1;
  x2(3,1) = 1;
  x2(3,2) = 1;
  x2(3,3) = 1;
  cout<<x2<<"\n\n";
  y2(3) = 0;
  cout<<y2<<"\n\n";
  gj = ((x2.transpose()*x2).inverse())*x2.transpose()*y2;
  cout<<gj<<"\n";
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
	//highestRating(map);
	//test();
}
