class projectile{
private:

public:
	int x, y;
	int speed;
	projectile(int cx, int cy);
	~projectile(){}
};

projectile::projectile(int cx, int cy){
	speed = 0;
	
}

