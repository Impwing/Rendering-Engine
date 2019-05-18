#ifndef PARTICLE_H
#define PARTICLE_H
#include "Containers.h"
#include <iostream>

#define TIMELIFE 10.0f
class Particle
{
public:
	Particle(glm::vec3 position, glm::vec3 velocity);
	~Particle();
	void update();
	void updateLife();


	glm::vec3 getVel();
	glm::vec4 getColor();
	bool remainingTime();
private:

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;

	float rotation;
	float time;
	//float lifetime = 5.0f;



};



#endif // !PARTICLE_H
