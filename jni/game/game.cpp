#include "game.h"

#include "math.h"
#include "vector"

Game::Game(float fieldWidth, float fieldHeight) : fieldWidth(fieldWidth), fieldHeight(fieldHeight) {
	timeRemaining = 0;
	combo = 0;
	score = 0;
	scale = fieldWidth;
	fieldRadius = vecLenF(fieldWidth, fieldHeight) / 2 + ASTEROID_RADIUS * scale;
	activeSpaceRadius = fieldRadius * 3 / 2;
	ship = new Ship(scale);
	state = GAME_STATE_IDLE;
}

float Game::getFieldWidth() {
	return fieldWidth;
}

float Game::getFieldHeight() {
	return fieldHeight;
}

int Game::isPlaying() {
	return state == GAME_STATE_ACTIVE;
}

void Game::changeTimeRemaining(float dTimeRemaining) {
	timeRemaining += dTimeRemaining;
	if (timeRemaining < 0)
		timeRemaining = 0;
	for (IPropertyObserver* observer : observers) {
		observer->handleTimeRemainingChange(dTimeRemaining);
	}
}

void Game::changeScore(int dScore) {
	this->score += dScore;
	for (IPropertyObserver* observer : observers) {
		observer->handleScoreChange(dScore);
	}
}

void Game::changeCombo(int dCombo) {
	this->combo += dCombo;
	for (IPropertyObserver* observer : observers) {
		observer->handleComboChange(dCombo);
	}
}

void Game::handleMotion(float distance, float dAngle) {
	for (IPropertyObserver* observer : observers) {
		observer->handleMotion(distance, dAngle);
	}
}

void Game::addObserver(IPropertyObserver* observer) {
	observers.push_back(observer);
}
