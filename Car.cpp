#include "Car.h"

// Initialization functions 
void Car::initVariables(sf::Vector2u windowSize)
{
	this->speed = 30.f;
	this->scale = 4.f;
	this->texture.loadFromFile("Textures/Samochody/car4.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setOrigin(this->sprite.getGlobalBounds().left + this->sprite.getGlobalBounds().width / 2.f, this->sprite.getGlobalBounds().top + this->sprite.getGlobalBounds().height / 2.f);
	this->sprite.setScale({ this->scale, this->scale });
	this->sprite.setPosition(static_cast<sf::Vector2f>(windowSize / 2u));
	this->speed_factor = 1.f;
}

// Constructors / Destructors
Car::Car(sf::Vector2u windowSize)
{
	this->initVariables(windowSize);
}

Car::~Car()
{
}

// Update functions
void Car::update(float dt)
{
	this->RP.update();
	this->updateMovement(dt);
	
}

sf::Vector2f Car::updateRotation()
{
	sf::Vector2f rotation = { static_cast<float>(this->RP.getNewLR()), static_cast<float>(this->RP.getNewFB()) };

	float kierunek = signbit(rotation.x) ? -1.f : 1.f;
	if (abs(rotation.x) < 2.f) {
		rotation.x = 0.f;
		this->sprite.setRotation(0.f);
	}
	else if (abs(rotation.x) < 8.f) {
		this->sprite.setRotation(8.f * kierunek);
	}
	else if (abs(rotation.x) < 16.f)
		this->sprite.setRotation(16.f * kierunek);
	else
		this->sprite.setRotation(24.f * kierunek);

	if (abs(rotation.y) < 1.f)
		rotation.y = 0.f;

	return rotation;
}

void Car::updateMovement(float dt)
{
	sf::Vector2f rotation = this->updateRotation();
	this->sprite.move({ rotation.x * dt * this->speed * this->speed_factor, rotation.y * dt * this->speed * this->speed_factor });
}

// Public functions
void Car::move(sf::Vector2f _offset)
{
	this->sprite.move(_offset);
}

void Car::move(sf::Vector2f _offset, float _dt)
{
	this->sprite.move(_offset * _dt * this->speed * this->speed_factor);
}

// Rendering the game
void Car::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}