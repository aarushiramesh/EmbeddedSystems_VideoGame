// images.h
// contains image buffers
// Jonathan Valvano, March 5, 2018
// Capture image dimensions from BMP files

#ifndef __images_h
#define __images_h


// *************************** Images ***************************
// the image of fenves for the starting screen, include background
// with the face, title, start
extern const unsigned short starterScreen[];

// *************************** Images ***************************
// still background for the course. It is solid walls on either side
// and a speedway down the middle
extern const unsigned short gameBackground[];

// *************************** Images ***************************
// it says running, but he will be still
// 
extern const unsigned short runningFenves[];

// *************************** Images ***************************
// first obstacle is a scooter on speedway. player can dodge by
// moving to the side
extern const unsigned short scooter[];

// *************************** Images ***************************
// speedway bricks falling from the sky, or verticle, or possibly
// random, literally whatever is easier
extern const unsigned short bricks[];


// *************************** Images ***************************
// bevo is THICCCCC and takes up the whole speedway. player must
// jump over
extern const unsigned short bevo[];

// *************************** Images ***************************
// solid black screen, says game over and your score
// 
extern const unsigned short gameOver[];

// *************************** Images ***************************
// the coins!
// 
extern const unsigned short coin[];


#endif /* __images_h */
