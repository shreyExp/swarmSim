#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>
#include <armadillo>
#include <math.h>

using namespace std;
using namespace Json;
class Agent{
	public:
		Agent();
		Agent(arma::Col<double> ri, arma::Col<double> vi);
		void setPosition(arma::Col<double> rs);
		void setVelocity(arma::Col<double> vs);
		void setAcceleration(arma::Col<double> as);
		void incrementPosition(arma::Col<double> velocity, double time);
		void incrementVelocity(arma::Col<double> acceleration, double time);
		arma::Col<double> getPosition();
		arma::Col<double> getVelocity();
		arma::Col<double> getAcceleration();
	private:
		arma::Col<double> r;
		arma::Col<double> v;
		arma::Col<double> a;
};
Agent::Agent(){
	arma::Col<double> c;
	c<<0<<0<<0;
	setPosition(c);
	setVelocity(c);
	setAcceleration(c);
}
Agent::Agent(arma::Col<double> ri, arma::Col<double> vi){
	setPosition(ri);
	setVelocity(vi);
	arma::Col<double> ai;
	ai<<0<<0<<0;
	setAcceleration(ai);
}
void Agent::setPosition(arma::Col<double> rs){
	r = rs; 	
}
void Agent::setVelocity(arma::Col<double> vs){
	v = vs;
}
void Agent::setAcceleration(arma::Col<double> as){
	a = as;
}
arma::Col<double> Agent::getPosition(){
	return r;
}
arma::Col<double> Agent::getVelocity(){
	return v;
}
arma::Col<double> Agent::getAcceleration(){
	return a;
}
void Agent::incrementPosition(arma::Col<double> velocity, double time){
	r += time * velocity;
}
void Agent::incrementVelocity(arma::Col<double> acceleration, double time){
	v += time * acceleration;
}



class Sim{
	public:
		Sim(const char*, const char*);
		void simPrint();
		void execute();
	private:
		Agent *A;
		Obstruction *Obs;
		int numberOfAgents;
		int numberOfObstructions;
};

Sim::Sim(const char* filenameAgents, const char* filenameObstructions){
	ifstream in;
	in.open(filenameAgents);
	Value root;
	in>>root;
	in.close();
	numberOfAgents = root["count"].asInt();
	const int size = count;
	A = new Agent[size];
	arma::Col<double> C;
	for(int i = 0; i < size; i++){
		C<<root["agents"][i]["position"][0].asDouble()<<root["agents"][i]["position"][1].asDouble()
		<<root["agents"][i]["position"][2].asDouble();
		(A+i)->setPosition(C);
		C<<root["agents"][i]["velocity"][0].asDouble()<<root["agents"][i]["velocity"][1].asDouble()
		<<root["agents"][i]["velocity"][2].asDouble();
		(A+i)->setVelocity(C);
	}
	in.open(filenameObstruction);
	numberOfObstructions = root["count"].asInt();
	const int sizeOfObstructions = numberOfObstructions;
	for(int i = 0; i < sizeOfObstructions; i++){
		C<<root["Obstruction"][i]["position"][0].asDouble()<<root["Obstruction"][i]["position"][1].asDouble()
		<<root["Obstruction"][i]["position"][2].asDouble();
		(A+i)->setPosition(C);
	}
}

void Sim::simPrint(){
	for(int i = 0; i < count; i++){
		cout<<"Position of "<<i<<" :\n"<<(A+i)->getPosition()<<endl;
		cout<<"Velocity of "<<i<<" :\n"<<(A+i)->getVelocity()<<endl;
		cout<<"Acceleration of "<<i<<" :\n"<<(A+i)->getAcceleration()<<endl;
	}
}

void Sim::execute(){
	arma::Col<double> zero;
	zero<<0<<0<<0;
	for(int t = 0; t < totalSteps; t++){
		for(int i = 0; i <  numberOfAgents; i++){
			A[i].setAccelearation(zero);
			for(int k = 0; k < numberOfAgents; k++){
				if( i != k )
					A[i].incrementAcceleration(findEffectOfAgent(A[i], A[k]));
			}
			for(int j = 0; j < numberOfObstructions; j++){
				A[i].incrementAcceleration(findEffectOfObstruction(A[i], Obs[j]));
			}
			A[i].findNewParameters();
			A[i].recordPosition();
		}
	}
}
arma::Col<double> Sim::findEffectOfAgent(Agent a, Agent b){
	arma::Col<double>aPos, bPos;
	aPos = a.getPosition();
	bPos = b.getPosition();
	arma::Col<double> r = weightForAgentAttraction()*(bPos-aPos)/pow((bPos-aPos),3);
	r += weightForAgentRepulsion()*(aPos-bPos)/pow(norm(bPos-aPos),4);
	return r;
}
class Obstruction{
	public:
		Obstruction();
		Obstruction(arma::Col<double>);
		void setPosition(arma::Col<double>);
		arma::Col<double> getPosition();
	private:
		arma::Col<double> R;
};

Obstruction::Obstruction(){
	arma::Col<double> r;
	r<<0<<0<<0;
	setPosition(r);
}

Obstruction::Obstruction(arma::Col<double> r){
	setPosition(r);
}

void Obstruction::setPosition(arma::Col<double> r){
	R = r;
}

arma::Col<double> getPosition(){
	return R;
}



int main(int argc, char** argv){
	 Swarm birds("agents.json");
	 birds.swarmPrint();
}
