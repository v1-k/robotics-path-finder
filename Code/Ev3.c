/*
 \file		Test.c
 \author	${user}
 \date		${date}
 \brief		Simple Hello World! for the Ev3
*/

#include <ev3.h>

#include "coordinates.h"

const int R90 = -90;
const int L90 = 90;
const float  MPI = 22/7;

char getDirection(int coordinate[2]){
    switch(coordinate[0]) {
        case 0:
            switch(coordinate[1]){
                case 1:
                    return 'R';
                case -1:
                    return 'L';
            }
        case 1:
            return 'D';
        case -1:
            return 'U';
   }
   return 'N';
}
int movement(int required[2],int direction[2],int movementArray[2]){

    char currentDirection = getDirection(direction);
    char requiredDirection = getDirection(required);
    printf("%c -->",currentDirection);
    printf("%c\t",requiredDirection);

    switch(currentDirection) {
        case 'R':
            switch(requiredDirection){
                case 'R':
                    movementArray[0]=0;
                    movementArray[1]=1;
                    return 0;

                case 'L':
                    movementArray[0]=2*R90;
                    movementArray[1]=1;
                    return 0;


                case 'D':
                    movementArray[0]=R90;
                    movementArray[1]=1;
                    return 0;


                case 'U':
                    movementArray[0]=L90;
                    movementArray[1]=1;
                    return 0;


            }
        case 'L':
            switch(requiredDirection){
                case 'R':
                    movementArray[0]=2*R90;
                    movementArray[1]=1;
                    return 0;


                case 'L':
                    movementArray[0]=0;
                    movementArray[1]=1;
                    return 0;


                case 'D':
                    movementArray[0]=L90;
                    movementArray[1]=1;
                    return 0;


                case 'U':
                    movementArray[0]=R90;
                    movementArray[1]=1;
                    return 0;

            }
        case 'D':
            switch(requiredDirection){
                case 'R':
                    movementArray[0]=L90;
                    movementArray[1]=1;
                    return 0;


                case 'L':
                    movementArray[0]=R90;
                    movementArray[1]=1;
                    return 0;


                case 'D':
                    movementArray[0]=0;
                    movementArray[1]=1;
                    return 0;


                case 'U':
                    movementArray[0]=2*R90;
                    movementArray[1]=1;
                    return 0;

            }
        case 'U':
            switch(requiredDirection){
                case 'R':
                    movementArray[0]=R90;
                    movementArray[1]=1;
                    return 0;


                case 'L':
                    movementArray[0]=L90;
                    movementArray[1]=1;
                    return 0;


                case 'D':
                    movementArray[0]=2*R90;
                    movementArray[1]=1;
                    return 0;


                case 'U':
                    movementArray[0]=0;
                    movementArray[1]=1;
                    return 0;

            }

   }
    return 0;
}
void rotateMotors(int speed,int motor,float distance,float degree_multiplier) {
	ResetRotationCount(motor);
	RotateMotor(motor,speed,distance/degree_multiplier);
}
void rotate(int speed,int motor,float distance,float degree_multiplier){
	ResetRotationCount(motor);
	OnFwdReg(motor,speed-5);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
			if (degree_multiplier*MotorRotationCount(motor) >= distance) {
				Off(motor);
				break;
		}
	}
	Wait(400);
	int resetFeedback = degree_multiplier*MotorRotationCount(motor) - distance;
	RotateMotor(motor,speed,resetFeedback);
}

void forwardSync(int speed,int motor,int motorA,int motorB,float distance,float degree_multiplier){
	ResetRotationCount(motorA);
	ResetRotationCount(motorB);
	OnFwdSync(motor,speed);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
			if (degree_multiplier*MotorRotationCount(motorA) >= distance) {
				Off(motorA);
				Off(motorB);
				break;
		}
	}

	int resetFeedbackA = degree_multiplier*MotorRotationCount(motorA) - distance;
	int resetFeedbackB = degree_multiplier*MotorRotationCount(motorB) - distance;
	TermPrintln("Motor A %d \n",resetFeedbackA);
	TermPrintln("Motor B %d \n",resetFeedbackB);

	RotateMotor(motorA,speed,resetFeedbackA);
	RotateMotor(motorB,speed,resetFeedbackB);
}
void rotateAB(int speed,int motor,int motorO,float rotation,float degree_multiplier){
	ResetRotationCount(motor);

	if (rotation>180){
		RotateMotor(motor,speed,180);
		rotation=rotation-180;
		rotateAB(speed,motor,motorO,rotation,degree_multiplier);
	}
	else{
		RotateMotor(motor,speed,rotation);
	}


//	ResetRotationCount(motor);
//	//ResetRotationCount(motorO);
//	OnFwdReg(motor,speed);
//	//OnRevReg(motorO,speed);
//	float distance_new = distance;
//	while(!ButtonIsDown(BUTTON_ID_ENTER)){
//		if (degree_multiplier*MotorRotationCount(motor) > distance_new) {
//			Off(motor);
//
//			break;
//		}
////		if (degree_multiplier*MotorRotationCount(motorO) > distance_new) {
////				Off(motorO);
////				Off(motor);
////				break;
////		}
//	}
//	Off(motor);
//	Wait(800);
//	int resetFeedback = degree_multiplier*MotorRotationCount(motor) - distance_new;
//	//int resetFeedbackO = degree_multiplier*MotorRotationCount(motorO)- distance_new;
//	TermPrintln("Motor  %d \n",resetFeedback);
//	//TermPrintln("Motor O %d \n",resetFeedbackO);
//	RotateMotor(motor,speed,resetFeedback);
//
//	//RotateMotor(motorO,speed,resetFeedbackO);
}
void moveRobot(int movementArray[2]){
	int m_a = OUT_A;  		// Motor A
	int m_b = OUT_B;  		// Motor B
	int m_ab = OUT_AB; 		// Motor A and B
	int motor_speed = 15;  	// Speed
	float constant_a = -0.017533;
	float constant_b = -0.017027	;
	float rotate_degree = 360;							// motor rotation 360 degree , one cycle

	float tire_dia_mm = 56;  							// Tire Diameter in centimeter
	float tire_width_mm = 28;							// Tire width in millimeter
	float tires_distance_cm = 9.3;						// distance between tires from inside edge to inside edge in centimeter
	float tire_total_dis_cm = tires_distance_cm + (tire_width_mm/2+tire_width_mm/2)/10;	// distance between tires from center to center in centimeter
	float rotation_distance_a_m = MPI*(tire_total_dis_cm/(2*100))+constant_a;	// Rotation distance for turning 90 degree in meter
	float rotation_distance_b_m = MPI*(tire_total_dis_cm/(2*100))+constant_b;	// Rotation distance for turning 90 degree in meter
	float tire_circum_m = MPI*(tire_dia_mm/1000); 		// Calculate Tire Circumference in meters
	float degree_multiplier_m = tire_circum_m/rotate_degree;		// multiplier for 1 degree rotation distance travel by tire in meter

	//TermPrintln("Hello World!");
	//TermPrintf("Press ENTER to exit");
	TermPrintf("constant_a %f\n",constant_a);
	TermPrintf("constant_b %f\n",constant_b);
	TermPrintf("rotation_distance_a_m %f\n",rotation_distance_a_m);
	TermPrintf("rotation_distance_b_m %f\n",rotation_distance_b_m);
	TermPrintf("degree_multiplier_m %f\n",degree_multiplier_m);

	float total_distance_m = 0.305/2;						// Distance to Travel straight



	int rotatation = movementArray[0];
	int run =  movementArray[1];
//	if (rotatation !=0 ){
//		int rotation = rotation_distance_a_m/degree_multiplier_m;
//		if (rotatation > 0){
//			rotateAB( motor_speed , m_a , m_b , rotation  , degree_multiplier_m );
//		}else{
//			rotateAB( motor_speed , m_a , m_b , rotation  , degree_multiplier_m );
//		}
//	}
	if (rotatation !=0 ){
		if (rotatation > 0){
			rotate( motor_speed,m_a, rotation_distance_a_m , degree_multiplier_m );
		}else{
			rotate( motor_speed,m_b, rotation_distance_b_m , degree_multiplier_m );
		}
		if (run !=0 ){
				forwardSync( motor_speed, m_ab, m_a, m_b, total_distance_m-0.1, degree_multiplier_m);
				//rotateMotors( motor_speed,m_ab, rotation_distance_m , degree_multiplier_m );
			}
	}
	else {
		forwardSync( motor_speed, m_ab, m_a, m_b, total_distance_m, degree_multiplier_m);
		//rotateMotors( motor_speed,m_ab, rotation_distance_m , degree_multiplier_m );
	}

}

int main(void)
{
	InitEV3();
	//int movementArray[2];
//	movementArray[0] =90;
//	movementArray[1] =0;
//	moveRobot( movementArray);
//	moveRobot( movementArray);
//	moveRobot( movementArray);
//	moveRobot( movementArray);
	init();

//	TermPrintln("tileWidth %.2f",tileWidth);
//	TermPrintln("tileHeight %.2f",tileHeight);
//	TermPrintln("n %d",n);
//	TermPrintln("m %d",m);
   // int gridStart[2]={round(start[1]/tileHeight),round(start[0]/tileWidth)};
   // int gridGoal[2] = {n - round(goal[1]/tileHeight),round(goal[0]/tileWidth)};
    float grid[n][m];
    buildMatrix(grid);

    //finding complete path
    int stopper = 1;
    int initDirection[2] = {0,1};

    int initCell[2] = {round(start[1]/tileHeight),round(start[0]/tileWidth)};
    int moveDirection[2];
    int nextCell[2]={round(start[1]/tileHeight),round(start[0]/tileWidth)};
    int movementArray[2];
    movementArray[0] =-90;
    movementArray[1] =0;
    moveRobot( movementArray);
    movementArray[0] =90;
    movementArray[1] =0;
    moveRobot( movementArray);
    //TermPrintln("Start Point: [%d,%d]\n",nextCell[0],nextCell[1]);
    while (grid[nextCell[0]][nextCell[1]]!=0){
        stopper = getNextCell(nextCell[0],nextCell[1],grid,nextCell,moveDirection);
        // printf("[%d,%d] , ",nextCell[0],nextCell[1]);
        movement(moveDirection,initDirection,movementArray);
        //TermPrintln("[%d,%d] \n",nextCell[0],nextCell[1]);

        moveRobot( movementArray);
        //TermPrintln("Hello World!");
        //printf("[%d,%d] , ",moveDirection[0],moveDirection[1]);

        initDirection[0]=moveDirection[0];
        initDirection[1]=moveDirection[1];
        initCell[0]=nextCell[0];
        initCell[1]=nextCell[1];


    }

    movementArray[0] =90;
	movementArray[1] =0;
	moveRobot( movementArray);
	RotateMotor(OUT_AB,15,140);
ButtonWaitForPress(BUTTON_ID_ENTER);
	FreeEV3();
	return 0;
}
