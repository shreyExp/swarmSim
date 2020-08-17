class Swarm{
	public:
		Swarm(const char* filename);
		void swarmPrint();
	private:
		Agent *S;
		int count;
		//AllObstructions Obstructions;

};

Swarm::Swarm(const char* filename){
	ifstream in;
	in.open(filename);
	Value root;
	in>>root;
	count = root["count"].asInt();
	const int size = count;
	S = new Agent[size];
	arma::Col<double> C;
	for(int i = 0; i < size; i++){
		C<<root["agents"][i]["position"][0].asDouble()<<root["agents"][i]["position"][1].asDouble()
		<<root["agents"][i]["position"][2].asDouble();
		(S+i)->setPosition(C);
		C<<root["agents"][i]["velocity"][0].asDouble()<<root["agents"][i]["velocity"][1].asDouble()
		<<root["agents"][i]["velocity"][2].asDouble();
		(S+i)->setVelocity(C);
	}
}

class AllObstructions{
	public:
		AllObstructions();
		AllObstructions(const char* filename);
	private:
		Obstruction *Obs;
		int count;
};

AllObstructions::AllObstructions(cont char* filename){
	ifstream in;
	in.open(filename);
	Value root;
	in>>root;
	count = root["count"].asInt();
	const int size = count;
	Obs = new Obstruction[size];
	arma::Col<double> C;
	for(int i = 0; i < size; i++){
		C<<root["obstruction"][i]["position"][0].asDouble()<<root["obstruction"][i]["position"][1].asDouble()
		<<root["obstruction"][i]["position"][2].asDouble();
		(Obs+i)->setPosition(C);
	}
}
