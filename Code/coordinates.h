
#include <math.h>
#include <stdio.h>
#include <string.h>
#define MAX_OBSTACLES 25 /* maximum number of obstacles */

const float width = 3.66;
const float height = 3.05;
const float originalWidth = 0.305;
const float originalHeight = 0.305;
float tileWidth,  tileHeight;
int n, m;

// #define MAX_OBSTACLES   25                      /* maximum number of obstacles */

int num_obstacles = 20;                         /* number of obstacles         */

double obstacle[MAX_OBSTACLES][2] =             /* obstacle locations          */
{{0.915, 0.305},{0.915, 0.61},{0.915, 0.915},{0.915, 1.219},
 {0.915, 1.524},{0.915, 1.829}, {1.829, 1.524},{1.829, 1.829},
 {1.829, 2.13},{1.829, 2.44},{1.829, 2.743},{1.829, 0.61},
 {2.13, 0.61}, {2.44, 0.61}, {2.743, 0.61},{2.743, 0.305},
 {2.743, 0.915},{2.743, 1.22},{2.743, 1.524},{2.743, 1.83},
 {-1,-1},{-1,-1},{-1,-1}, {-1,-1},{-1,-1}};

//double start[2] = {0.305, 0.61};          /* start location              */
double start[2] = {0.305, 2.44};          /* start location              */

//double goal[2] = {3.356, 1.524};
double goal[2] = {3.356, 1.526};
// const int num_obstacles = 13; /* number of obstacles */

void init(){
	  tileWidth = originalWidth /2.0;
	  tileHeight = originalHeight /2.0;
	  n = height/tileHeight;
	  m = width/tileWidth;
}

void setCheck(float grid[n][m], int i,int j,int val){
    if (0<=i && i<n && 0<=j && j<m){
        grid[i][j]= val;
    }
}

//direction array to specify the movement direction
int directions[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};

int getNextCell(int row,int col,float grid[n][m],int newCell[2], int moveDirection[2]){
	int d;
    for ( d = 0; d<4;d++){
        newCell[0] = row+directions[d][0];
        newCell[1] = col+directions[d][1];
        if (0<=newCell[0] && newCell[0]<n && 0<=newCell[1] && newCell[1]<m){
            if (grid[row][col] > grid[newCell[0]][newCell[1]]){
                moveDirection[0]=directions[d][0];
                moveDirection[1]=directions[d][1];
                return 1;
            }
        }
    }
    return 0;
}

int buildMatrix(float grid[n][m]){
    // float grid[n][m];
	int i,j,d;

	int infinity = n*m*100;
    for(i=0;i<n;i++){
        for( j=0;j<m;j++){
            grid[i][j]=m*n;
        }
    }
    //Marking Obstacles on gird
    for( i=0;i<num_obstacles;i++){
        int cell[2] = {n - round(obstacle[i][1]/tileHeight),round(obstacle[i][0]/tileWidth)-1};

         setCheck(grid,cell[0]-1,cell[1]-1,infinity);
         setCheck(grid,cell[0]-2,cell[1]-1,infinity);
         setCheck(grid,cell[0]-1,cell[1]-2,infinity);
         setCheck(grid,cell[0]-2,cell[1]-2,infinity);

         // grid[cell[0]-1][cell[1]-1] = infinity;
         // grid[cell[0]-2][cell[1]-1] = infinity;
         // grid[cell[0]-1][cell[1]-2] = infinity;

         setCheck(grid,cell[0]-1,cell[1],infinity);
         setCheck(grid,cell[0]-2,cell[1],infinity);
         setCheck(grid,cell[0]-1,cell[1]+1,infinity);
         setCheck(grid,cell[0]-2,cell[1]+1,infinity);

         // grid[cell[0]-1][cell[1]] = infinity;
         // grid[cell[0]-2][cell[1]] = infinity;
         // grid[cell[0]-1][cell[1]+1] = infinity;

         setCheck(grid,cell[0],cell[1]-1,infinity);
         setCheck(grid,cell[0],cell[1]-2,infinity);
         setCheck(grid,cell[0]+1,cell[1]-1,infinity);
         setCheck(grid,cell[0]+1,cell[1]+1,infinity);

         // grid[cell[0]][cell[1]-1] = infinity;
         // grid[cell[0]][cell[1]-2] = infinity;
         // grid[cell[0]+1][cell[1]-1] = infinity;

         setCheck(grid,cell[0],cell[1],infinity);
         setCheck(grid,cell[0]+1,cell[1],infinity);
         setCheck(grid,cell[0],cell[1]+1,infinity);
         setCheck(grid,cell[0]+1,cell[1]-2,infinity);
    }

    //Marking Goal on gird
    int cell[2] = {n - round(goal[1]/tileHeight),round(goal[0]/tileWidth)-1};
    grid[cell[0]-1][cell[1]-1] = 0;
    grid[cell[0]-1][cell[1]] = 0;
    grid[cell[0]][cell[1]-1] = 0;
    grid[cell[0]][cell[1]] = 0;

    int queueA[(m+n)*4][2],queueB[(m+n)*4][2];

    //adding goal points to the queue
    memcpy(queueA[0],(int[2]){cell[0]-1,cell[1]-1},sizeof(int[2]));
    memcpy(queueA[1],(int[2]){cell[0]-1,cell[1]},sizeof(int[2]));
    memcpy(queueA[2],(int[2]){cell[0],cell[1]-1},sizeof(int[2]));
    memcpy(queueA[3],(int[2]){cell[0],cell[1]},sizeof(int[2]));

    int counter = 4;
    //level wise bfs to populate the distance in the grid from goal
    while ( counter > 0) {
        int tempCount = 0;
        for ( i = 0;i<counter;i++){
            for (d = 0; d<4;d++){
                int cell[2] = {queueA[i][0]+directions[d][0],queueA[i][1]+directions[d][1]};
                if (0<=cell[0] && cell[0]<n && 0<=cell[1] && cell[1]<m){
                    if (grid[cell[0]][cell[1]]!= infinity && grid[queueA[i][0]][queueA[i][1]]+1<grid[cell[0]][cell[1]]){
                        grid[cell[0]][cell[1]] = grid[queueA[i][0]][queueA[i][1]]+1;
                        queueB[tempCount][0]= cell[0];
                        queueB[tempCount][1]= cell[1];
                        tempCount++;
                    }
                }
            }
            
        }
        counter = tempCount;
        memcpy(queueA, queueB, sizeof(queueB));
    }

//    print grid
//    for (i = 0; i < n; i++) {
//            for (j = 0; j < m; j++) {
//                printf("%.0f \t", grid[i][j]);
//            }
//            printf("\n");
//    }

    
    return 0;
}
