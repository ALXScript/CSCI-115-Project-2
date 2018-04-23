//Player Class

class Player {
public:
	//boolean to check living status of player
	bool isAlive;

	//boolean to check if player is in the moving state or the firing state
	bool isFiring;

	//int to keep track of how many arrows the player has
	int arrowCount;

	//int to keep track of the current position on the X axis
	int xPosition;

	//int to keep track of the current position in the Y axis
	int yPosition;

	/*int to find out what direction the player is currenty facing (for firing function)
	1 = up
	2 = down
	3 = left
	4 = right*/
	int facingDirection;

	//function for moving
	//function for firing


private:
};